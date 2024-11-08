#include "application.h"
#include <QDebug>
#include <sstream>

Application::Application(int &argc, char **argv, TcpServer *otherServer, Polynom& otherPolynom, quint16 port)
    : QCoreApplication(argc, argv), server(otherServer), polynom(otherPolynom)
{
    server->startServer(port);
    connect(server, &TcpServer::messageReceived, this, &Application::onMessageReceived);
}

Application::~Application() = default;

void Application::onMessageReceived(QTcpSocket *clientSocket, const QByteArray &message)
{
    qDebug() << "Получено сообщение от клиента:" << message;
    QString messageStr = QString::fromUtf8(message);
    processMessage(clientSocket, messageStr);
}

void Application::processMessage(QTcpSocket *clientSocket, const QString &message)
{
    QString response = "";
    std::istringstream stream(message.toStdString());
    std::stringstream ss;
    std::string command;
    stream >> command;

    if (command == "changeAn") {
        double realPart, imagPart;
        stream >> realPart >> imagPart;
        polynom.setAn(number(realPart, imagPart));
        polynom.show(ss);
        polynom.show(ss, 0);
        response += QString::fromStdString(ss.str());
    }
    else if (command == "addRoot") {
        double realPart, imagPart;
        stream >> realPart >> imagPart;
        polynom.addRoot(number(realPart, imagPart));
        polynom.show(ss);
        polynom.show(ss, 0);
        response += QString::fromStdString(ss.str());
    }
    else if (command == "changeRoot") {
        double realPart, imagPart;
        int index;
        stream >> realPart >> imagPart >> index;
        polynom.setRoot(index, number(realPart, imagPart));
        polynom.show(ss);
        polynom.show(ss, 0);
        response += QString::fromStdString(ss.str());
    }
    else if (command == "rootsResize") {
        size_t newSize;
        stream >> newSize;
        polynom.resize(newSize);
        polynom.show(ss);
        polynom.show(ss, 0);
        response += QString::fromStdString(ss.str());
    }
    else if (command == "evaluate") {
        double realPart, imagPart;
        stream >> realPart >> imagPart;
        number res = polynom.evaluate(number(realPart, imagPart));
        response += "p(";
        if (realPart != 0.0 || imagPart != 0.0) {
            if (realPart != 0.0) {
                response += QString::number(realPart);
            }
            if (imagPart != 0.0) {
                if (imagPart > 0.0 && realPart != 0.0 ) response += " + ";
                response += QString::number(imagPart) + 'i';
            }
        }
        else {
            response += QString::number(realPart);
        }
        response += ") = ";
        if (res.getRe() != 0 || res.getIm() != 0) {
            if (res.getRe() != 0) {
                response += QString::number(res.getRe());
            }
            if (res.getIm() != 0) {
                if (res.getIm() > 0) response += " + ";
                else response += " - ";
                response += QString::number(std::abs(res.getIm())) + "i";
            }
        }
        else {
            response += "0";
        }
    }

    server->sendMessage(clientSocket, response);
}
