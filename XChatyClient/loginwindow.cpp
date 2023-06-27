#include "loginwindow.h"
#include "chatyDef.h"
#include <QCryptoGraphicHash>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_user = nullptr;
	m_logClient = nullptr;
	m_chatWindow = nullptr;
	ui.passwdEdit->setEchoMode(QLineEdit::Password);
}

LoginWindow::~LoginWindow()
{
	SAFE_DELETE(m_user);
}

void LoginWindow::OnLogin()
{
	QString name = ui.nameEdit->text();
	QString passwd = ui.passwdEdit->text();
	if(name.isEmpty() || passwd.isEmpty())
	{
		QMessageBox::warning(this, "Warning", "Name or passwd is empty!");
		return;
	}
	QString passwd_md5 = QCryptographicHash::hash(passwd.toUtf8(), QCryptographicHash::Md5).toHex();
	// TODO: check name and passwd
	m_user = new User;
	m_user->userName = name;
	m_user->passwd = passwd_md5;
	
	// 验证通过
	m_chatWindow = new XChatyClient(m_user);
	this->close();

}
