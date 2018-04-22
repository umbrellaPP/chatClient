#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>
#include <QAbstractSocket>
#include "../define.h"
#include "../util/jsonparser.h"
#include "handler.h"

#define Net Client::getInstance()

class Client: public QObject
{
    Q_OBJECT
public:
    static Client* getInstance() {
        static Client *_instance = new Client;
        return _instance;
    }

    void init();
    void initWeb();

    void sendPackage(Code code, QJsonObject obj);  // 发送数据包
    bool isConnected() {
        return this->socket->state() == QAbstractSocket::ConnectedState;
    }

    Handler *handler;

public slots:
    void connected();
    void connectError(QAbstractSocket::SocketError err);
    void disconnected();
    void receivePackage();  // 接收数据包

signals:
    void connectedToServer();

private:
    Client();

    QTcpSocket *socket;
};

#endif // CLIENT_H
