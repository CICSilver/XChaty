#include "loginwindow.h"
#include "chatyDef.h"

LoginWindow::LoginWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_user = nullptr;
}

LoginWindow::~LoginWindow()
{}
