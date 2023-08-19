#include "loginwindow.h"
#include "hv.h"
#include <QCryptoGraphicHash>
#include <QMessageBox>
#include <QDataStream>

LoginWindow::LoginWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_user = nullptr;
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
						// 注册成功，直接登录
						PostUserInfo(protoc::REQ_REGIST);
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
}

void LoginWindow::onResp(const hv::TcpClient::TSocketChannelPtr& channel, hv::Buffer* buffer)
{
	protoc::ChatyMessage msg = protoc::ConvertBuf2ChatyMsg(buffer);
	if (!msg.pMsgBody) XERR("Response Error, response type:", msg.msgHead.msgType);
	switch (msg.msgHead.msgType)
	{
		case protoc::RESP_LOGIN_OK:
		{

			break;
		}
		case protoc::RESP_LOGIN_FAILED:
		case protoc::RESP_REGIST_OK:
		case protoc::RESP_REGIST_FAILED:
	}
}

void LoginWindow::PostUserInfo(protoc::ReqType _msgType)
{
	XLOG("PostUserInfo");
	hv::TcpClient loginClient;
	int connfd = loginClient.createsocket(m_serverPort, m_serverHost.toUtf8().constData());
	if (connfd < 0)
	{
		QMessageBox::warning(this, "warning", "server connecting failed");
		return;
	}
	protoc::LoginMsg loginMsg;
	loginMsg.user = m_user;
	QByteArray ba = protoc::Serrialize(protoc::ChatyMessage(_msgType, &loginMsg));

	loginClient.onConnection = [this] (const hv::TcpClient::TSocketChannelPtr& channel)
	{
		XLOG("connect to", channel->id(), channel->peeraddr());
	};
	loginClient.onMessage = &LoginWindow::onResp;

	loginClient.start();
	loginClient.send(ba.data(), ba.length());
	ui.logBtn->setEnabled(false);
	ui.regBtn->setEnabled(false);
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

	PostUserInfo();

	// 验证通过
	//m_chatWindow = new XChatyClient(m_user);
	//this->close();

}


