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
    m_HB_interval = 1000;   // TEST:每秒一次心跳包
    if(!InitConnect())
        m_chatHelper->AppendMsg("connect failed!");
    m_chatHelper->OverWriteMsg("connect success!");
    connect(ui.sendBtn, &QPushButton::clicked, this, &XChatyClient::SendMsg);
    connect(ui.sendEdit, &QLineEdit::returnPressed, this, &XChatyClient::SendMsg);
}

XChatyClient::~XChatyClient()
{
    if (m_client)
    {
        if(m_client->isConnected())
            m_client->stop();
    }
    SAFE_DELETE(m_client);
    //SAFE_DELETE(m_user);
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
        QByteArray ba((char*)buf->data());
        ChatyMsg msg = protoc::DeSerrialize(ba);
        ChatMsg* chat_msg = dynamic_cast<ChatMsg*>(msg.pMsgBody.get());
        m_chatHelper->PostMsg(this, chat_msg->chatMsg);
    };
    m_client->onConnection = [this](const hv::SocketChannelPtr& channel)
    {
        QString str = channel->isConnected() ? "connected" : "disconnected";
        m_chatHelper->PostMsg(this, QString("connfd: %1 is %2.").arg(channel->fd()).arg(str));
        // 设置心跳包,每分钟发送一次
        hv::setInterval(m_HB_interval, [channel](hv::TimerID timerId)
        {
			if (channel->isConnected()) 
			{
                ChatyMsg msg(protoc::REQ_HB);
                QByteArray ba = protoc::Serrialize(msg);
				channel->write(ba.constData(), ba.size());
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
    //qDebug() << "SendMsg";
    XLOG(m_client->isConnected());
    if (m_client->isConnected())
    {
        ChatMsg chatMsg;
        chatMsg.chatMsg = ui.sendEdit->text();
        chatMsg.userName = m_user->userName;
        chatMsg.chatRoom = 0;
        ChatyMsg msg(protoc::REQ_CHAT, &chatMsg);
        XLOG(msg.msgHead.msgType);
        QByteArray ba = protoc::Serrialize(msg);
        XLOG(ba.size());

        m_client->send(ba.constData(), ba.size());

    }
    else
    {
        SAFE_DELETE(m_client);
        m_client = nullptr;
        // 尝试重连
        InitConnect();
    }
}
