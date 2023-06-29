#include "loginwindow.h"
#include "chatyDef.h"
#include "hv.h"
#include <QCryptoGraphicHash>
#include <QMessageBox>
#include <QDataStream>

LoginWindow::LoginWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_user = nullptr;
	m_logClient = nullptr;
	m_chatWindow = nullptr;
	m_regWindow = nullptr;
	m_serverHost = "127.0.0.1";
	m_serverPort = 1234;
	ui.passwdEdit->setEchoMode(QLineEdit::Password);

	connect(ui.logBtn, &QPushButton::clicked, this, &LoginWindow::OnLogin);
	connect(ui.regBtn, &QPushButton::clicked, this, [this] ()
			{
				if (m_regWindow == nullptr)
				{
					if (!m_user) m_user = new chaty::User;
					m_regWindow = new RegWindow(m_user);
					connect(m_regWindow, &RegWindow::RegisterSuccess, this, [this] ()
							{
								// 注册成功
								PostUserInfo(chaty::REGIST);
								m_regWindow->close();
								m_regWindow = nullptr;
							});
				}
				m_regWindow->show();
			});
}

LoginWindow::~LoginWindow()
{
	SAFE_DELETE(m_user);
	SAFE_DELETE(m_logClient);
}

void LoginWindow::PostUserInfo(chaty::ClientMsgType msgType)
{
	XLOG("PostUserInfo");
	if (!m_logClient)
		m_logClient = new hv::TcpClient;
	int connfd = m_logClient->createsocket(m_serverPort, m_serverHost.toUtf8().constData());
	if (connfd < 0)
	{
		QMessageBox::warning(this, "warning", "server connecting failed");
		return;
	}
	chaty::LoginMessage msg(*m_user, msgType);
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);
	stream << msg;
	m_logClient->onWriteComplete = [this] (const hv::TcpClient::TSocketChannelPtr& channel, hv::Buffer* buffer)
	{
		XLOG("[Client] Write done");
		m_logClient->closesocket();
	};
	m_logClient->onConnection = [this] (const hv::TcpClient::TSocketChannelPtr& channel)
	{
		XLOG("connect to", channel->id());
	};
	system("pause");
	m_logClient->start();
	m_logClient->send(buffer, buffer.size());
}



void LoginWindow::OnLogin()
{
	
	QString name = ui.nameEdit->text();
	QString passwd = ui.passwdEdit->text();
	if(name.isEmpty() || passwd.isEmpty())
	{
		QMessageBox::warning(this, "Warning", "Name or passwd is empty!");
		return;
	}
	QString passwd_md5 = QCryptographicHash::hash(passwd.toUtf8(), QCryptographicHash::Md5).toHex();
	// TODO: check name and passwd
	m_user = new chaty::User;
	m_user->userName = name;
	m_user->passwd = passwd_md5;

	//// 验证通过
	//m_chatWindow = new XChatyClient(m_user);
	//this->close();

}
