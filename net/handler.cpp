#include "handler.h"
#include <QDebug>
#include <QMessageBox>

Handler::Handler() {
    // 注册路由
    this->handlerDict.insert(Code::S2C_LOGIN, Handler::handleLogin);
    this->handlerDict.insert(Code::S2C_REGISTER, Handler::handleRegister);
    this->handlerDict.insert(Code::S2C_GET_FRIENDS_LIST, Handler::handleGetFriendList);
    this->handlerDict.insert(Code::S2C_ADD_FRIEND, Handler::handleAddFriend);
    this->handlerDict.insert(Code::S2C_REMOVE_FRIEND, Handler::handleRemoveFriend);
    this->handlerDict.insert(Code::S2C_SEND_MSG, Handler::handleSendMsg);
}

void Handler::handle(QTcpSocket *socket, QByteArray package) {
    Code code;
    QJsonObject obj;
    qDebug() << "客户端接收package: " << package;
    JsonTool->getPackageData(package, &code, &obj);
    HandleFunc handleFunc = this->handlerDict.value(code);
    if (handleFunc) {
        (this->*handleFunc)(socket, obj);
    } else {
        qDebug() << "找不到对应的处理器处理 code " + code;
    }
    qDebug() << code << obj;
}

void Handler::handleLogin(QTcpSocket *socket, QJsonObject data) {
    double result = data.value("result").toDouble();
    if (result == 0){
        QString accountId = data.value("accountId").toString();
        QString userName = data.value("userName").toString();
        // todo 登录成功
        QMessageBox::information(nullptr, "登录成功", "登录成功");
    } else if (result == 1){
        QMessageBox::information(nullptr, "登录失败", "不存在此用户");
    } else if (result == 2){
        QMessageBox::information(nullptr, "登录失败", "用户密码错误");
    }
}

void Handler::handleRegister(QTcpSocket *socket, QJsonObject data) {
    double result = data.value("result").toDouble();
    if (result == 0) {
        QMessageBox::information(nullptr, "注册成功", "注册成功");
    }else if (result == 1) {
        QMessageBox::information(nullptr, "注册失败", "注册失败");
    }
}

void Handler::handleGetFriendList(QTcpSocket *socket, QJsonObject data) {

}

void Handler::handleAddFriend(QTcpSocket *socket, QJsonObject data) {

}

void Handler::handleRemoveFriend(QTcpSocket *socket, QJsonObject data) {

}

void Handler::handleSendMsg(QTcpSocket *socket, QJsonObject data) {

}


