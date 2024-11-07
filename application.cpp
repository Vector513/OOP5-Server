#include "Application.h"
#include <QDebug>

// Конструктор
Application::Application(int &argc, char **argv, TcpServer *server, Polynom& otherPolynom)
    : QCoreApplication(argc, argv), server(server), polynom(otherPolynom)
{
    // Соединяем сигнал о получении сообщения с соответствующим слотом
    server->startServer(10001);
    connect(server, &TcpServer::messageReceived, this, &Application::onMessageReceived);
}

// Деструктор
Application::~Application()
{
    // Здесь можно очистить ресурсы, если это нужно
}

// Метод exec
void Application::exec(TcpServer* otherServer)
{
    // Логика, если необходимо использовать другой сервер
    if (otherServer) {
        qDebug() << "Используем другой сервер.";
        // Примените настройки для другого сервера, если нужно
    }

    // Запускаем основной цикл обработки событий
    QCoreApplication::exec();
}

// Слот обработки полученных сообщений
void Application::onMessageReceived(QTcpSocket* clientSocket, const QString& message)
{
    qDebug() << "Получено сообщение от сервера:" << message;
    // Здесь вы можете обрабатывать сообщение, например, выполнить какие-то действия
    // в зависимости от содержания сообщения.
    processMessage(clientSocket, message);
}

void Application::processMessage(QTcpSocket *clientSocket, const QString &message)
{
    // Логика обработки сообщения
    // Например, добавляем префикс и отправляем обратно
    QString response = "Обработанное сообщение: " + message;


    // Отправляем обработанное сообщение обратно всем клиентам (или конкретному клиенту)
    server->sendMessage(clientSocket, response);
}