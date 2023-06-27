#include "mainwindow.h"
#include "chatwindowutilty.h"
#include "customevent.h"
#include "htime.h"
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <qeventloop.h>
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
    QString recvMsg = QString::fromLocal8Bit((char*)buf->data(), buf->size());
    if (recvMsg.size() == 1 && recvMsg.at(0) == 'h')
    {
        // 心跳包
        channel->write("s");
        return;
    }
    
    m_chatHelper->PostMsg(this, recvMsg);
    // echo
     m_server->broadcast(recvMsg.toStdString());
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
