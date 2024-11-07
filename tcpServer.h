#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QByteArray>
#include <QDebug>
#include <QHostAddress>
#include <QMap>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>

class TcpServer : public QObject
{
    Q_OBJECT

public:
    explicit TcpServer(QObject *parent = nullptr);
    void startServer(quint16 port);
    void sendMessage(QTcpSocket *clientSocket, const QString &response);
    //void onDataReceived();

signals:
    void messageReceived(QTcpSocket *clientSocket, const QString &message);

private slots:
    void onNewConnection();
    void onDataReceived();
    void onClientDisconnected();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);
    void onBytesWritten(qint64 bytes);

private:
    QTcpServer *server;
    QMap<QTcpSocket *, QString> clientSockets; // Храним сокеты и адреса клиентов
};

#endif // TCPSERVER_H
