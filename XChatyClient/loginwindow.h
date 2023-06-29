#pragma once

#include <QMainWindow>
#include "ui_loginwindow.h"
#include "chatyDef.h"
#include "xchatyclient.h"
#include "regwindow.h"
class LoginWindow : public QMainWindow
{
	Q_OBJECT

public:
	LoginWindow(QWidget *parent = nullptr);
	~LoginWindow();

private:
	void PostUserInfo(chaty::ClientMsgType msgType);

public slots:
	void OnLogin();
private:
	Ui::LoginWindowClass ui;
	chaty::User* m_user;

	XChatyClient* m_chatWindow;
	RegWindow* m_regWindow;

	int m_serverPort;
	QString m_serverHost;
};
