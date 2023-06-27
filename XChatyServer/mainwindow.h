#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "TcpServer.h"
#include <qdebug.h>
class ChatWindowUtilty;
class mainwindow : public QMainWindow
{
    Q_OBJECT
        
public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

    void InitChatServer();

    void customEvent(QEvent* e) override;

private:
    void onMsg(const hv::SocketChannelPtr& channel, hv::Buffer* buf);
    void onConnection(const hv::SocketChannelPtr& channel);
private:
    Ui::mainwindowClass ui;
    hv::TcpServer* m_server;
    ChatWindowUtilty* m_chatHelper;
};
