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
#include "UserTableDAO.h"
mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_server = nullptr;
    m_chatHelper = ChatWindowUtilty::GetInstance(ui.chatEdit);
    InitChatServer();

    auto StartServer = [this] ()
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
        m_server->setThreadNum(4);
        m_server->start();
        m_chatHelper->OverWriteMsg(QString("Listening on port: %1").arg(port));
    };
    StartServer();
    connect(ui.startBtn, &QPushButton::clicked, this, StartServer);
    //QThreadPool::globalInstance()->start()
}

mainwindow::~mainwindow()
{
    XLOG("~mainwindow()");
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
        qDebug() << "onMsg";
		onMsg(channel, buf);
	};
    
}

void mainwindow::onMsg(const hv::SocketChannelPtr& channel, hv::Buffer* buf)
{
    QByteArray qbuffer((char*)buf->data(), buf->size());
    XLOG(buf->size());
    ChatyMessage msg = protochat::DeSerrialize(qbuffer);

    RespMsg response;
    switch (msg.msgHead.msgType)
    {
        case chaty_client::REQ_LOGIN:
        {
            // login request
            LoginMsg* login_msg = dynamic_cast<LoginMsg*>(msg.pMsgBody.get());
            
            XLOG(login_msg->user.userName);
            break;
        }
        case chaty_client::REQ_REGIST:
        {
            // regist request
            RegistMsg* reg_msg = dynamic_cast<RegistMsg*>(msg.pMsgBody.get());
            break;
        }
        case chaty_client::REQ_CHAT:
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
}

void mainwindow::onConnection(const hv::SocketChannelPtr& channel)
{
    QString state = channel->isConnected() ? "connected" : "disconnected";
    // 设置超时时间
    channel->setKeepaliveTimeout(1500);
    m_chatHelper->PostMsg(this, QString("[Server] conn id: %1 %2")
        .arg(channel->id())
        .arg(state));
}

void mainwindow::LoginCheck(chaty::User _user)
{
    UserTableDAO dao;
    dao.GetUser(_user.id);
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
