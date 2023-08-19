#pragma once

#include <QWidget>
#include "ui_regwindow.h"
#include "UserModel.h"

class RegWindow : public QWidget
{
	Q_OBJECT

public:
	RegWindow(chaty::User* user, QWidget *parent = nullptr);
	~RegWindow();

signals:
	void RegisterSuccess();

public slots:
	void onConfirmBtnClicked();
private:
	Ui::RegWindowClass ui;
	chaty::User* m_user;
};
