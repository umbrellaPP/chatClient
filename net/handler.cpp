#include "handler.h"
#include <QDebug>

Handler::Handler() {
    this->handlerDict.insert(Code::S2C_LOGIN, Handler::handleLogin);
    this->handlerDict.insert(Code::S2C_REGISTER, Handler::handleRegister);
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
    qDebug() << code << obj;
}

void Handler::handleLogin(QTcpSocket *socket, QJsonObject data) {
    qDebug() << "handleLogin";
}

void Handler::handleRegister(QTcpSocket *socket, QJsonObject data) {
    qDebug() << "handleRegister";
}


