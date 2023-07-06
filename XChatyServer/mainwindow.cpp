#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <qeventloop.h>
#include <QThreadpool>
#include <QDataStream>
#include "mainwindow.h"
#include "chatwindowutilty.h"
#include "customevent.h"
#include "htime.h"
#include "SqlHelper.h"
mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_server = nullptr;
    m_chatHelper = ChatWindowUtilty::GetInstance(ui.chatEdit);
    InitChatServer();
    connect(ui.startBtn, &QPushButton::clicked, this, [this]()
        {
            QString port = ui.portEdit->text();
            if (port.isEmpty())
            {
                m_chatHelper->AppendMsg("Port is empty!");
                return;
            }
            int listenfd = m_server->createsocket(port.toInt());
            if (listenfd < 0)
            {
				m_chatHelper->AppendMsg("Create socket failed!");
				return;
			}
            m_server->setThreadNum(1);
            m_server->start();
            m_chatHelper->OverWriteMsg(QString("Listening on port: %1").arg(port));
        });
    //QThreadPool::globalInstance()->start()
}

mainwindow::~mainwindow()
{
    SAFE_DELETE(m_server);
}

void mainwindow::InitChatServer()
{
    m_server = new hv::TcpServer();
    m_server->onConnection = [this](const hv::SocketChannelPtr& channel)
    {
		onConnection(channel);
	};
    m_server->onMessage = [this](const hv::SocketChannelPtr& channel, hv::Buffer* buf)
    {
		onMsg(channel, buf);
	};
    
}

void mainwindow::onMsg(const hv::SocketChannelPtr& channel, hv::Buffer* buf)
{
    QByteArray qbuffer((char*)buf->data(), buf->size());
    ChatyMessage msg = protochat::DeSerrialize(qbuffer);
    switch (msg.msgHead.msgType)
    {
        case chaty::MSG_LOGIN:
        {
            // login request
            LoginMsg* login_msg = dynamic_cast<LoginMsg*>(msg.pMsgBody.get());
            XLOG(login_msg->user.userName);
            break;
        }
        case chaty::MSG_REGIST:
        {
            // regist request
            RegistMsg* reg_msg = dynamic_cast<RegistMsg*>(msg.pMsgBody.get());
            break;
        }
        case chaty::MSG_CHAT:
        {
            // chat request
            ChatMsg* chat_msg = dynamic_cast<ChatMsg*>(msg.pMsgBody.get());
            XLOG(chat_msg->chatRoom, chat_msg->userName, chat_msg->chatMsg);
            m_chatHelper->PostMsg(this, chat_msg->chatMsg);
            break;
        }
        default:
            break;
    }
    //XLOG(str, str.user.userName);
    //m_chatHelper->PostMsg(this, msg.user.userName);
    
    //QString recvMsg = QString::fromLocal8Bit((char*)buf->data(), buf->size());
    //if (recvMsg.size() == 1 && recvMsg.at(0) == 'h')
    //{
    //    // 心跳包
    //    channel->write("s");
    //    return;
    //}

    //m_chatHelper->PostMsg(this, recvMsg);
    //// echo
    // m_server->broadcast(recvMsg.toStdString());
}

void mainwindow::onConnection(const hv::SocketChannelPtr& channel)
{
    QString state = channel->isConnected() ? "connected" : "disconnected";
    // 设置超时时间
    channel->setKeepaliveTimeout(60000);
    m_chatHelper->PostMsg(this, QString("[Server] conn id: %1 %2")
        .arg(channel->id())
        .arg(state));
}

void mainwindow::customEvent(QEvent* e)
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
    default: break;
    }
}
