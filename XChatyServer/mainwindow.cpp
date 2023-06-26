#include "mainwindow.h"

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_server = nullptr;
}

mainwindow::~mainwindow()
{
    SAFE_DELETE(m_server);
}

void mainwindow::InitChatServer()
{

}
