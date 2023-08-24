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
    m_userDao = new UserTableDAO;
    InitChatServer();
    ui.startBtn->setEnabled(false);
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
}

mainwindow::~mainwindow()
{
    XLOG("~mainwindow()");
    SAFE_DELETE(m_server);
    SAFE_DELETE(m_userDao);
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
    ChatyMessage msg = protoc::ConvertBuf2ChatyMsg(buf);
    if (!msg.pMsgBody) return;
    RespMsg response;
    switch (msg.msgHead.msgType)
    {
        case protoc::REQ_LOGIN:
        {
            // login request
            LoginMsg* login_msg = dynamic_cast<LoginMsg*>(msg.pMsgBody.get());
            // default distribute to room 0
            XLOG("user ip:", channel.get()->peeraddr());
            chaty::User user = login_msg->user;
            XLOG(user.id, user.passwd, user.userName);
            protoc::ResponseType respType = LoginCheck(login_msg->user)? 
                protoc::RESP_LOGIN_SUCCESS : 
                protoc::RESP_LOGIN_FAILED;
            response.respHead.msgType = protoc::RESP_LOGIN_SUCCESS;
            break;
        }
        case protoc::REQ_REGIST:
        {
            // regist request
            RegistMsg* reg_msg = dynamic_cast<RegistMsg*>(msg.pMsgBody.get());
            chaty::User user = reg_msg->user;
            XLOG(user.userName, user.passwd);
            break;
        }
        case protoc::REQ_CHAT:
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

bool mainwindow::LoginCheck(chaty::User _user)
{
    UserTableDAO dao;
    chaty::User* pUser = dao.GetUser(_user.id);
    if (pUser)
    {
        XLOG(pUser->userName);
    }
    else
    {
        XLOG("user not exists");
    }

    return true;
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
