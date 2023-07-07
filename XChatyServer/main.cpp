#include "mainwindow.h"
#include <QtWidgets/QApplication>

// DEBUG
#include "hv.h"
#include "TcpServer.h"
#include "SqlHelper.h"
#include "chatyDef.h"
#include "UserTableDAO.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainwindow w;
    w.show();
    return a.exec();
}