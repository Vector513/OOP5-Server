#ifndef APPLICATION_H
#define APPLICATION_H

#include <QCoreApplication>
#include <QTcpSocket>
#include "tcpServer.h"
#include "polynom.h"

class Application : public QCoreApplication
{
    Q_OBJECT

public:
    Application(int &argc, char **argv, TcpServer *otherServer, Polynom &otherPolynom, quint16 port = 10001);
    ~Application();

private slots:
    void onMessageReceived(QTcpSocket *clientSocket, const QString &message);

private:
    TcpServer *server;
    Polynom &polynom;

    void processMessage(QTcpSocket *clientSocket, const QString &message);
};

#endif // APPLICATION_H
