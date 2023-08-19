#include "regwindow.h"
#include "XLog.h"
#include <QMessageBox>
#include <QCryptoGraphicHash>
RegWindow::RegWindow(chaty::User* user, QWidget *parent)
	: QWidget(parent),
	m_user(user)
{
	ui.setupUi(this);

	connect(ui.cancelBtn, &QPushButton::clicked, this, &QWidget::close);
	connect(ui.confirmBtn, &QPushButton::clicked, this, &RegWindow::onConfirmBtnClicked);
}

RegWindow::~RegWindow()
{}

void RegWindow::onConfirmBtnClicked()
{
	QString name = ui.nameEdit->text();
	QString passwd = ui.passwdEdit->text();
	QString passwd_re = ui.repeatEdit->text();

	if (name.isEmpty() || passwd.isEmpty())
	{
		QMessageBox::warning(this, "warning", QStringLiteral("����д������Ϣ"));
		return;
	}
	if (passwd != passwd_re)
	{
		QMessageBox::warning(this, "warning", QStringLiteral("�������벻һ��"));
		return;
	}
	m_user->userName = name;
	m_user->passwd = QCryptographicHash::hash(passwd.toUtf8(), QCryptographicHash::Md5).toHex();
	XLOG(m_user->userName);
	emit RegisterSuccess();
}

