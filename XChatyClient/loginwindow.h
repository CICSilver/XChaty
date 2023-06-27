#pragma once

#include <QMainWindow>
#include "ui_loginwindow.h"
class User;
class LoginWindow : public QMainWindow
{
	Q_OBJECT

public:
	LoginWindow(QWidget *parent = nullptr);
	~LoginWindow();

private:
	Ui::LoginWindowClass ui;
	User* m_user;
};
