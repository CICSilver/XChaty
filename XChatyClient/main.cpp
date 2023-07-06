#include "xchatyclient.h"
#include "loginwindow.h"
#include "chatyDef.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ===== DEBUG =====
    chaty::User user;
    user.id = "0";
    user.userName = "admin";
    user.passwd = "12345";
    // =================
    XChatyClient w(&user);
    w.show();
    return a.exec();
}
