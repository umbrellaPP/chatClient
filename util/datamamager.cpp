#include "datamamager.h"
#include "net/client.h"

DataMamager::DataMamager() {

}

void DataMamager::init() {
    connect(Net->handler, SIGNAL(logined(QJsonObject)), this, SLOT(logined(QJsonObject)));
}

void DataMamager::setUserData(QString id, QString name) {
    accountId = id;
    userName = name;
}

void DataMamager::setCurrentChatting(QString id, ChattingType type) {
    currentChatting.id = id;
    currentChatting.type = type;
}

void DataMamager::logined(QJsonObject obj) {
    QString accountId = obj.value("accountId").toString();
    QString userName = obj.value("userName").toString();
    this->setUserData(accountId, userName);
}
