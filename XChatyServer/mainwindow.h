#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "TcpServer.h"
#include "chatyDef.h"
#include "connectinfo.h"
#include "hv.h"
#include "hsocket.h"
class ChatWindowUtilty;
class mainwindow : public QMainWindow
{
    Q_OBJECT
    typedef protochat::ChatyMessage ChatyMessage;
    typedef protochat::ChatMsg ChatMsg;
    typedef protochat::LoginMsg LoginMsg;
    typedef protochat::RegistMsg RegistMsg;
    typedef protochat::RespMsg RespMsg;
public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

    void InitChatServer();

    void customEvent(QEvent* e) override;

private:
    void onMsg(const hv::SocketChannelPtr& channel, hv::Buffer* buf);
    void onConnection(const hv::SocketChannelPtr& channel);

    void LoginCheck(chaty::User _user);
private:
    Ui::mainwindowClass ui;
    hv::TcpServer* m_server;
    ChatWindowUtilty* m_chatHelper;
    QMap<QString, uint32_t>map; // <username, channel_id>
};
