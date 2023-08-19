#pragma once

#include <QMainWindow>
#include "ui_loginwindow.h"
#include "xchatyclient.h"
#include "regwindow.h"

// ============== login ===============
// 1. open tcpclient and connect to server
// 2. send user info to server
// 3. wait for response
// *4. RESP_LOGIN_OK / RESP_LOGIN_FAILED
// 5. close tcpclient
// ====================================

// ============== regist ===============
// 1. open tcpclient and connect to server
// 2. send user info to server
// 3. wait for response
// *4. RESP_REGIST_OK / RESP_REGIST_FAILED
// 5. close tcpclient
// ====================================

class LoginWindow : public QMainWindow
{
	Q_OBJECT

public:
	LoginWindow(QWidget *parent = nullptr);
	~LoginWindow();

	static void onResp(const hv::TcpClient::TSocketChannelPtr& channel, hv::Buffer* buffer);
private:
	void PostUserInfo(chaty_client::ReqType _msgType = chaty_client::REQ_LOGIN);
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
