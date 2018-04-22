#include "handler.h"
#include <QDebug>
#include <QMessageBox>
#include "net/client.h"

Handler::Handler() {
    // 注册路由
    this->handlerDict.insert(CodeNS::S2C_LOGIN, Handler::handleLogin);
    this->handlerDict.insert(CodeNS::S2C_REGISTER, Handler::handleRegister);
    this->handlerDict.insert(CodeNS::S2C_UPDATE_FRIENDS_LIST, Handler::handleUpdateFriendList);
    this->handlerDict.insert(CodeNS::S2C_FIND_FRIEND, Handler::handleFindFriend);
    this->handlerDict.insert(CodeNS::S2C_ADD_FRIEND, Handler::handleAddFriend);
    this->handlerDict.insert(CodeNS::S2C_REMOVE_FRIEND, Handler::handleRemoveFriend);
    this->handlerDict.insert(CodeNS::S2C_SEND_MSG, Handler::handleSendMsg);
    this->handlerDict.insert(CodeNS::S2C_NEW_MSG, Handler::handleNewMsg);

    this->handlerDict.insert(CodeNS::S2C_CREATE_GROUP, Handler::handleCreateGroup);
    this->handlerDict.insert(CodeNS::S2C_UPDATE_GROUPS_LIST, Handler::handleUpdateGroupsList);
    this->handlerDict.insert(CodeNS::S2C_FIND_GROUP, Handler::handleFindGroup);
    this->handlerDict.insert(CodeNS::S2C_JOIN_GROUP, Handler::handleJoinGroup);
    this->handlerDict.insert(CodeNS::S2C_UPDATE_MEMBERS, Handler::handleUpdateMembers);
    this->handlerDict.insert(CodeNS::S2C_NEW_GROUP_MSG, Handler::handleNewGroupMsg);
    this->handlerDict.insert(CodeNS::S2C_QUIT_GROUP, Handler::handleQuitGroup);
}

void Handler::handle(QTcpSocket *socket, QByteArray package) {
    Code code;
    QJsonObject obj;
    JsonTool->getPackageData(package, &code, &obj);
    HandleFunc handleFunc = this->handlerDict.value(code);
    if (handleFunc) {
        (this->*handleFunc)(socket, obj);
    } else {
        qDebug() << "找不到对应的处理器处理 code " + code;
    }
    qDebug() << "package: " << code << obj;
}

void Handler::handleLogin(QTcpSocket *socket, QJsonObject data) {
    double result = data.value("result").toDouble();
    if (result == 0) {
        QString accountId = data.value("accountId").toString();
        QString userName = data.value("userName").toString();
        QJsonObject package;
        package.insert("accountId", accountId);
        // todo 登录成功
        emit logined(data);
        Net->sendPackage(CodeNS::C2S_GET_FRIENDS_LIST, package);
        Net->sendPackage(CodeNS::C2S_GET_GROUP_LIST, package);
    } else if (result == 1) {
        QMessageBox::information(nullptr, "登录失败", "不存在此用户");
    } else if (result == 2) {
        QMessageBox::information(nullptr, "登录失败", "用户密码错误");
    } else if (result == 3) {
        QMessageBox::information(nullptr, "登录失败", "用户已经在线，请勿重复登录");
    }
}

void Handler::handleRegister(QTcpSocket *socket, QJsonObject data) {
    emit registered(data);
}

void Handler::handleUpdateFriendList(QTcpSocket *socket, QJsonObject data) {
    emit updateFriendList(data);
}

void Handler::handleFindFriend(QTcpSocket *socket, QJsonObject data) {
    double result = data.value("result").toDouble();
    if (result == 0) {  // 查找成功
        emit userFound(data);
    } else if (result == 1){  // 查无此用户
        emit userNotFound();
    }
}

void Handler::handleAddFriend(QTcpSocket *socket, QJsonObject data) {
    double result = data.value("result").toDouble();
    emit addFriendResult(data);
}

void Handler::handleRemoveFriend(QTcpSocket *socket, QJsonObject data) {
    emit removeFriend(data);
}

void Handler::handleSendMsg(QTcpSocket *socket, QJsonObject data) {
    emit sendMsg(data);
}

void Handler::handleNewMsg(QTcpSocket *socket, QJsonObject data) {
    emit newMsg(data);
}

void Handler::handleCreateGroup(QTcpSocket *socket, QJsonObject data) {
    emit createGroup(data);
}

void Handler::handleUpdateGroupsList(QTcpSocket *socket, QJsonObject data) {
    emit updateGroupList(data);
}

void Handler::handleFindGroup(QTcpSocket *socket, QJsonObject data) {
    emit findGroup(data);
}

void Handler::handleJoinGroup(QTcpSocket *socket, QJsonObject data) {
    emit joinGroup(data);
}

void Handler::handleUpdateMembers(QTcpSocket *socket, QJsonObject data) {
    emit updateMembers(data);
}

void Handler::handleNewGroupMsg(QTcpSocket *socket, QJsonObject data) {
    emit newGroupMsg(data);
}

void Handler::handleQuitGroup(QTcpSocket *socket, QJsonObject data)  {
    emit quitGroup(data);
}


