#include "mainwindow.h"
#include <QMessageBox>
mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_server = nullptr;

    connect(ui.startBtn, QPushButton::clicked, this, [this]()
        {
            QString port = ui.portEdit->text();
            if (port.isEmpty())
            {
                QMessageBox::critical(this, "Error", "Port is empty!");
                return;
            }
            int listenfd = m_server->createsocket(port.toInt());
            if (listenfd < 0)
            {
				QMessageBox::critical(this, "Error", "Listen failed!");
				return;
			}


        });
}

mainwindow::~mainwindow()
{
    SAFE_DELETE(m_server);
}

void mainwindow::InitChatServer()
{
    
}

void mainwindow::onMsg(const hv::SocketChannelPtr& channel, hv::Buffer* buf)
{
	// echo
	
}
