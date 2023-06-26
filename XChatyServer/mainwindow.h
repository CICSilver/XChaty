#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "TcpServer.h"

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

    void InitChatServer();

private:
    Ui::mainwindowClass ui;
    hv::TcpServer* m_server;
};
