#include "server.h"
#include <QDebug>

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent), clientSocket(nullptr) {}

void TcpServer::startServer(quint16 port) {
    if (listen(QHostAddress::Any, port)) {
        qDebug() << "Server started on port" << port;
    } else {
        qDebug() << "Server could not start!";
    }
}

void TcpServer::incomingConnection(qintptr socketDescriptor) {
    clientSocket = new QTcpSocket(this);
    clientSocket->setSocketDescriptor(socketDescriptor);

    connect(clientSocket, &QTcpSocket::readyRead, this, &TcpServer::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &TcpServer::onDisconnected);

    qDebug() << "Client connected with descriptor" << socketDescriptor;
}

void TcpServer::onReadyRead() {
    QByteArray data = clientSocket->readAll();
    qDebug() << "Received:" << data;

    // Отправляем данные обратно клиенту
    clientSocket->write("Echo: " + data);
}

void TcpServer::onDisconnected() {
    qDebug() << "Client disconnected";
    clientSocket->deleteLater();
}
