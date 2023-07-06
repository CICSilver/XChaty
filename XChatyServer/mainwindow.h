#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "TcpServer.h"
#include "chatyDef.h"
class ChatWindowUtilty;
class mainwindow : public QMainWindow
{
    Q_OBJECT
    typedef protochat::ChatyMessage ChatyMessage;
    typedef protochat::ChatMsg ChatMsg;
    typedef protochat::LoginMsg LoginMsg;
    typedef protochat::RegistMsg RegistMsg;
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
