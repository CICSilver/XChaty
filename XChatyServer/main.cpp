#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include "hv.h"
#include "TcpServer.h"
using namespace hv;

#define TEST_TLS 0
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainwindow w;
    w.show();
    return a.exec();
}