#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include "hv.h"
#include "TcpServer.h"
using namespace hv;

#define TEST_TLS 0
//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    mainwindow w;
//    w.show();
//    return a.exec();
//}

int main(int argc, char* argv[]) {
    int port = 5678;

    hlog_set_level(LOG_LEVEL_DEBUG);

    TcpServer srv;
    int listenfd = srv.createsocket(port);
    if (listenfd < 0) {
        printf("listenfd:%d < 0", listenfd);
        system("Pause");
        return -20;
    }
    printf("server listen on port %d, listenfd=%d ...\n", port, listenfd);
    srv.onConnection = [](const SocketChannelPtr& channel) {
        std::string peeraddr = channel->peeraddr();
        if (channel->isConnected()) {
            printf("%s connected! connfd=%d id=%d tid=%ld\n", peeraddr.c_str(), channel->fd(), channel->id(), currentThreadEventLoop->tid());
        }
        else {
            printf("%s disconnected! connfd=%d id=%d tid=%ld\n", peeraddr.c_str(), channel->fd(), channel->id(), currentThreadEventLoop->tid());
        }
    };
    srv.onMessage = [](const SocketChannelPtr& channel, Buffer* buf) {
        // echo
        printf("< %.*s\n", (int)buf->size(), (char*)buf->data());
        channel->write(buf);
    };
    srv.setThreadNum(4);
    srv.setLoadBalance(LB_LeastConnections);

#if TEST_TLS
    hssl_ctx_opt_t ssl_opt;
    memset(&ssl_opt, 0, sizeof(hssl_ctx_opt_t));
    ssl_opt.crt_file = "cert/server.crt";
    ssl_opt.key_file = "cert/server.key";
    ssl_opt.verify_peer = 0;
    srv.withTLS(&ssl_opt);
#endif

    srv.start();

    std::string str;
    while (std::getline(std::cin, str)) {
        if (str == "close") {
            srv.closesocket();
        }
        else if (str == "start") {
            srv.start();
        }
        else if (str == "stop") {
            srv.stop();
            break;
        }
        else {
            srv.broadcast(str.data(), str.size());
        }
    }
    system("Pause");
    return 0;
}