#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_xchatyclient.h"
#include "TcpClient.h"
#include "chatwindowutilty.h"
class XChatyClient : public QMainWindow
{
    Q_OBJECT

public:
    XChatyClient(QWidget *parent = nullptr);
    ~XChatyClient();


    bool InitConnect();

    void customEvent(QEvent* e) override;

public slots:
    void SendMsg();

private:
    Ui::XChatyClientClass ui;
    hv::TcpClient* m_client;
    ChatWindowUtilty* m_chatHelper;
};
