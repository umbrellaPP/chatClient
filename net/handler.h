#ifndef HANDLER_H
#define HANDLER_H

#include "../define.h"
#include "../util/jsonparser.h"
#include <QObject>
#include <QByteArray>
#include <QJsonObject>
#include <QMap>
#include <QTcpSocket>

class Handler;
typedef void (Handler::*HandleFunc)(QTcpSocket *socket, QJsonObject data);

class Handler: public QObject
{
    Q_OBJECT
public:
    Handler();

    void handle(QTcpSocket *socket, QByteArray package);

private:
    QMap<Code, HandleFunc> handlerDict;

    void handleLogin(QTcpSocket *socket, QJsonObject data);
    void handleRegister(QTcpSocket *socket, QJsonObject data);
    void handleUpdateFriendList(QTcpSocket *socket, QJsonObject data);
    void handleFindFriend(QTcpSocket *socket, QJsonObject data);
    void handleAddFriend(QTcpSocket *socket, QJsonObject data);
    void handleRemoveFriend(QTcpSocket *socket, QJsonObject data);
    void handleSendMsg(QTcpSocket *socket, QJsonObject data);
    void handleNewMsg(QTcpSocket *socket, QJsonObject data);

    void handleCreateGroup(QTcpSocket *socket, QJsonObject data);
    void handleUpdateGroupsList(QTcpSocket *socket, QJsonObject data);
    void handleFindGroup(QTcpSocket *socket, QJsonObject data);
    void handleJoinGroup(QTcpSocket *socket, QJsonObject data);
    void handleUpdateMembers(QTcpSocket *socket, QJsonObject data);
    void handleNewGroupMsg(QTcpSocket *socket, QJsonObject data);
    void handleQuitGroup(QTcpSocket *socket, QJsonObject data);


signals:
    void registered(QJsonObject data);
    void logined(QJsonObject data);
    void userFound(QJsonObject data);
    void userNotFound();
    void addFriendResult(QJsonObject data);
    void removeFriend(QJsonObject data);
    void updateFriendList(QJsonObject data);
    void sendMsg(QJsonObject data);
    void newMsg(QJsonObject data);

    void createGroup(QJsonObject data);
    void updateGroupList(QJsonObject data);
    void findGroup(QJsonObject data);
    void joinGroup(QJsonObject data);
    void updateMembers(QJsonObject data);
    void newGroupMsg(QJsonObject data);
    void quitGroup(QJsonObject data);
};

#endif // HANDLER_H
