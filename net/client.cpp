﻿#include "client.h"
#include <QDebug>

const QString ip = "127.0.0.1";
const int port = 8090;

Client::Client() {

}

void Client::init() {
    this->handler = new Handler();
    this->initWeb();
}

void Client::initWeb() {
    this->socket = new QTcpSocket(this);
    connect(this->socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(this->socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectError(QAbstractSocket::SocketError)));
    connect(this->socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    this->socket->connectToHost(QHostAddress(ip), port);
}

void Client::sendPackage(Code code, QJsonObject obj) {
    QByteArray package = JsonTool->generatePackage(code, obj);
    this->socket->write(package);
    this->socket->write(";");
}

void Client::connected() {
    qDebug() << "连接服务器成功";
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(receivePackage()));
    emit this->connectedToServer();
}

void Client::connectError(QAbstractSocket::SocketError err) {
    qDebug() << "链接出错 " << err;
}

void Client::disconnected() {
    qDebug() << "连接断开";
}

void Client::receivePackage() {
    QByteArray content = this->socket->readAll();
    QList<QByteArray> list = content.split(';');
    list.pop_back();
    for (QByteArray package: list){
        if (!package.isEmpty()) {
            this->handler->handle(this->socket, package);
        } else {
            qDebug() << "不存在数据包";
        }
    }
}
