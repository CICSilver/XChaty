#pragma once

#include <QMainWindow>
#include "ui_loginwindow.h"
#include "chatyDef.h"
#include "xchatyclient.h"
class LoginWindow : public QMainWindow
{
	Q_OBJECT

public:
	LoginWindow(QWidget *parent = nullptr);
	~LoginWindow();

public slots:
	void OnLogin();

private:
	Ui::LoginWindowClass ui;
	XChatyClient* m_chatWindow;
	chaty::User* m_user;

	hv::TcpClient* m_logClient;
};
