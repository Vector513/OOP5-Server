#include "tcpServer.h"

TcpServer::TcpServer(QObject *parent)
    : QObject(parent), server(new QTcpServer(this))
{
    // Подключаем сигнал для обработки новых соединений
    connect(server, &QTcpServer::newConnection, this, &TcpServer::onNewConnection);
}

void TcpServer::startServer(quint16 port)
{
    if (server->listen(QHostAddress::Any, port)) {
        qDebug() << "Сервер запущен на порту" << port;
        QHostAddress serverIp = server->serverAddress();
        qDebug() << "Сервер слушает на IP-адресе:" << serverIp.toString();
    } else {
        qCritical() << "Ошибка запуска сервера!";
    }
}

void TcpServer::sendMessage(QTcpSocket* clientSocket, const QString &response)
{
    if (clientSocket->state() == QAbstractSocket::ConnectedState) {
        qDebug() << "Отправка данных:" << response;

        clientSocket->write(response.toUtf8());

        // Синхронная отправка: ждем, пока все данные будут записаны
        if (!clientSocket->waitForBytesWritten(5000)) {
            qWarning() << "Ошибка при отправке данных:" << clientSocket->errorString();
        } else {
            qDebug() << "Данные отправлены!";
        }
    } else {
        qWarning() << "Нет подключения к серверу!";
    }
}

void TcpServer::onNewConnection()
{
    QTcpSocket *clientSocket = server->nextPendingConnection();

    // Подключаем сигналы для сокета
    connect(clientSocket, &QTcpSocket::readyRead, this, &TcpServer::onDataReceived);
    connect(clientSocket, &QTcpSocket::disconnected, this, &TcpServer::onClientDisconnected);
    connect(clientSocket, &QTcpSocket::errorOccurred, this, &TcpServer::onErrorOccurred);
    connect(clientSocket, &QTcpSocket::bytesWritten, this, &TcpServer::onBytesWritten);

    // Сохраняем информацию о клиенте
    clientSockets[clientSocket] = clientSocket->peerAddress().toString();

    qDebug() << "Новый клиент подключился:" << clientSockets[clientSocket];
}

void TcpServer::onDataReceived()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    QByteArray data = clientSocket->readAll();
    qDebug() << "Получены данные от клиента" << clientSockets[clientSocket] << ":" << data;

    if (data.isEmpty()) {
        qWarning() << "Нет данных от клиента, возможно, клиент отключился.";
    } else {
        // Отправляем ответ клиенту
        //QString response = "Ответ от сервера: " + QString::fromUtf8(data);
        //clientSocket->write(response.toUtf8());
        emit messageReceived(clientSocket, data);
        clientSocket->flush();
        qDebug() << "Ответ отправлен клиенту" << clientSockets[clientSocket];
    }
}

void TcpServer::onClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    qDebug() << "Клиент отключился:" << clientSockets[clientSocket];
    clientSockets.remove(clientSocket);  // Удаляем клиента из списка
    clientSocket->deleteLater();
}

void TcpServer::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        qWarning() << "Удалённый хост закрыл соединение:" << clientSockets[clientSocket];
        break;
    case QAbstractSocket::HostNotFoundError:
        qWarning() << "Хост не найден:" << clientSockets[clientSocket];
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qWarning() << "Подключение отклонено:" << clientSockets[clientSocket];
        break;
    default:
        qWarning() << "Ошибка сокета у клиента" << clientSockets[clientSocket] << ":" << clientSocket->errorString();
        break;
    }
    clientSockets.remove(clientSocket);  // Удаляем клиента из списка
    clientSocket->deleteLater();
}

void TcpServer::onBytesWritten(qint64 bytes)
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    qDebug() << bytes << "байт отправлено клиенту" << clientSockets[clientSocket];
}
