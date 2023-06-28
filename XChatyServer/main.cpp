#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include "hv.h"
#include "TcpServer.h"
#include "SqlHelper.h"
#include "chatyDef.h"
using namespace hv;

#define TEST_TLS 0
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainwindow w;
    w.show();
    return a.exec();

    //SqlHelper* helper = SqlHelper::GetInstance();
    //QString dbPath = chaty::GetDataBasePath(CHATY_DB_NAME);
    //XLOG(dbPath);
    //helper->Open(dbPath);
    //helper->CreateTable("UserTable", "id INT PRIMARY KEY NOT NULL, name TEXT NOT NULL, passwd TEXT NOT NULL");
    //helper->InsertData("UserTable", "1, 'admin', '12345'");
    //helper->Query("UserTable", "id, name, passwd", "id > 0");
    //system("pause");
    return 0;
}