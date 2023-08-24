#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_xchatyclient.h"
#include "TcpClient.h"
#include "chatwindowutilty.h"
#include "Protoc.h"
class XChatyClient : public QMainWindow
{
    Q_OBJECT
    typedef protoc::ChatyMessage ChatyMsg;
    typedef protoc::ChatMsg ChatMsg;

public:
    XChatyClient(chaty::User* user, QWidget *parent = nullptr);
    ~XChatyClient();


    bool InitConnect();

    void customEvent(QEvent* e) override;

public slots:
    void SendMsg();

private:
    Ui::XChatyClientClass ui;
    hv::TcpClient* m_client;
    ChatWindowUtilty* m_chatHelper;
    chaty::User* m_user;
    quint16 m_HB_interval;
    int m_hb_timerId;
};
