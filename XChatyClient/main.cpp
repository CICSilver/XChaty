#include "xchatyclient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XChatyClient w;
    w.show();
    return a.exec();
}
