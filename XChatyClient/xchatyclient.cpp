#include "xchatyclient.h"
#include "customevent.h"
#include "htime.h"
#include <QDebug>
XChatyClient::XChatyClient(chaty::User* user, QWidget *parent)
    : QMainWindow(parent),
    m_user(user)
{
    ui.setupUi(this);
    m_client = nullptr;
    m_chatHelper = ChatWindowUtilty::GetInstance(ui.chatEdit);
    if(!InitConnect())
        m_chatHelper->AppendMsg("connect failed!");
    m_chatHelper->OverWriteMsg("connect success!");
    connect(ui.sendBtn, &QPushButton::clicked, this, &XChatyClient::SendMsg);
    connect(ui.sendEdit, &QLineEdit::returnPressed, this, &XChatyClient::SendMsg);
}

XChatyClient::~XChatyClient()
{
    SAFE_DELETE(m_client);
    SAFE_DELETE(m_user);
}

bool XChatyClient::InitConnect()
{
    if(!m_client)
		m_client = new hv::TcpClient();

	QString host = ui.hostEdit->text();
	QString port = ui.portEdit->text();
    if (host.isEmpty() || port.isEmpty())
    {
		m_chatHelper->AppendMsg("host or port is empty!");
		return false;
	}

    int connfd = m_client->createsocket(port.toInt(), host.toStdString().c_str());
    if (connfd < 0)
        return false;

    m_client->onMessage = [this](const hv::SocketChannelPtr& channel, hv::Buffer* buf)
    {
        QString recvMsg = QString::fromLocal8Bit((char*)buf->data(), buf->size());
        m_chatHelper->PostMsg(this, recvMsg);
    };
    m_client->onConnection = [this](const hv::SocketChannelPtr& channel)
    {
        QString str = channel->isConnected() ? "connected" : "disconnected";
        m_chatHelper->PostMsg(this, QString("connfd: %1 is %2.").arg(channel->fd()).arg(str));
        // 设置心跳包,每分钟发送一次
        hv::setInterval(60000, [channel](hv::TimerID timerId)
            {
			    if (channel->isConnected())
				    {
					    channel->write("h");
				    }
				    else
				    {
					    hv::killTimer(timerId);
				    }
            });
    };

    m_client->start();
    return true;
}

void XChatyClient::customEvent(QEvent* e)
{
    switch (e->type())
    {
        case qEventRecvMsg:
        {
            QStringEvent* event = dynamic_cast<QStringEvent*>(e);
            if (event)
            {
                m_chatHelper->AppendMsg(event->m_msg);
            }
            e->accept();
            break;
        }
        default:break;
    }
}

void XChatyClient::SendMsg()
{
    if (m_client->isConnected())
    {
        QString sendMsg = ui.sendEdit->text();
        m_client->send(sendMsg.toStdString().c_str(), sendMsg.length());
    }
}
