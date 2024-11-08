#include <QCoreApplication>
#include "application.h"
#include "tcpServer.h"

int main(int argc, char *argv[])
{
    TcpServer server;
    Polynom polynom;
    QCoreApplication a(argc, argv);
    Application app(argc, argv, &server, polynom);

    return a.exec();
}
