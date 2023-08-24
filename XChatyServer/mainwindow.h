#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "TcpServer.h"
#include "connectinfo.h"
#include "hv.h"
#include "hsocket.h"
#include "Protoc.h"

class UserTableDAO;
class ChatWindowUtilty;
class mainwindow : public QMainWindow
{
    Q_OBJECT
    typedef protoc::ChatyMessage ChatyMessage;
    typedef protoc::ChatMsg ChatMsg;
    typedef protoc::LoginMsg LoginMsg;
    typedef protoc::RegistMsg RegistMsg;
    typedef protoc::RespMsg RespMsg;
public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

    void InitChatServer();

    void customEvent(QEvent* e) override;

private:
    void onMsg(const hv::SocketChannelPtr& channel, hv::Buffer* buf);
    void onConnection(const hv::SocketChannelPtr& channel);

    bool LoginCheck(chaty::User _user);
private:
    Ui::mainwindowClass ui;
    hv::TcpServer* m_server;
    ChatWindowUtilty* m_chatHelper;
    UserTableDAO* m_userDao;
    QMap<quint32, QList<chaty::User>> m_chatRoomMap; // <channel_id, user_list>
};
