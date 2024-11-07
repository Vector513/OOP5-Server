#ifndef APPLICATION_H
#define APPLICATION_H

#include <QCoreApplication>// Для наследования от QCoreApplication
#include "TcpServer.h"
#include "polynom.h"

class Application : public QCoreApplication
{
    Q_OBJECT

public:
    explicit Application(int &argc, char **argv, TcpServer *server, Polynom& otherPolynom);
    ~Application() override;

    void exec(TcpServer* otherServer);

private slots:
    void onMessageReceived(QTcpSocket* clientSocket, const QString& message);  // Слот для обработки сообщений от сервера
    void processMessage(QTcpSocket* clientSocket, const QString &message);     // Логика обработки сообщений

private:
    TcpServer *server;
    Polynom& polynom;
};

#endif // APPLICATION_H
