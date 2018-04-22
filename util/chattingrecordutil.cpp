#include "chattingrecordutil.h"
#include <QJsonDocument>
#include <QFile>
#include "net/client.h"


ChattingRecordUtil::ChattingRecordUtil(QObject *parent) : QObject(parent) {

}

void ChattingRecordUtil::init(){
    connect(Net->handler, SIGNAL(newMsg(QJsonObject)), this, SLOT(newMsg(QJsonObject)));
    connect(Net->handler, SIGNAL(sendMsg(QJsonObject)), this, SLOT(sendMsg(QJsonObject)));
    connect(Net->handler, SIGNAL(newGroupMsg(QJsonObject)), this, SLOT(newGroupMsg(QJsonObject)));
    connect(Net->handler, SIGNAL(removeFriend(QJsonObject)), this, SLOT(removeFriend(QJsonObject)));
    connect(Net->handler, SIGNAL(quitGroup(QJsonObject)), this, SLOT(quitGroup(QJsonObject)));
}

QJsonArray ChattingRecordUtil::readRecord(ChattingType type,
                               QString currentId, QString chattingId) {
    QString fileName = this->generateFileName(type, currentId  , chattingId);
    QFile file(fileName);
    if (!file.exists()){
        return QJsonArray();
    }
    file.open(QIODevice::ReadOnly);
    QJsonArray arr(QJsonDocument::fromJson(file.readAll()).array());
    return arr;
}

void ChattingRecordUtil::writeRecord(ChattingType type, QString currentId,
                         QString chattingId, QString newMsg) {
    QString fileName = this->generateFileName(type, currentId, chattingId);
    QFile file(fileName);
    file.open(QIODevice::ReadWrite);
    // 先读
    QJsonArray recResult = this->readRecord(type, currentId, chattingId);
    // 后写
    recResult.push_back(newMsg);
    file.write(QJsonDocument(recResult).toJson());
}

void ChattingRecordUtil::removeRecord(ChattingType type, QString currentId, QString chattingId) {
    QString fileName = this->generateFileName(type, currentId, chattingId);
    QFile file(fileName);
    file.remove();
}

void ChattingRecordUtil::newMsg(QJsonObject data) {
    double result = data.value("result").toDouble();
    if (result == 1) {
        return ;
    }
    ChattingType type = ChattingType::PERSONAL;
    QString fromId = data.value("fromId").toString();
    QString toId = data.value("toId").toString();
    QString fromUserName = data.value("fromUserName").toString();
    QString msg = data.value("msg").toString();

    this->writeRecord(type, toId, fromId, fromUserName+": "+msg);
}

void ChattingRecordUtil::sendMsg(QJsonObject data) {
    double result = data.value("result").toDouble();
    if (result == 1) {
        return ;
    }
    ChattingType type = ChattingType::PERSONAL;
    QString fromId = data.value("fromId").toString();
    QString toId = data.value("toId").toString();
    QString fromUserName = data.value("fromUserName").toString();
    QString msg = data.value("msg").toString();

    this->writeRecord(type, fromId, toId, fromUserName+": "+msg);
}

void ChattingRecordUtil::newGroupMsg(QJsonObject data) {
    ChattingType type = ChattingType::GROUP;
    QString accountId = DataMgr->getAccountId();
    QString groupId = data.value("groupId").toString();
    QString fromUserName = data.value("fromUserName").toString();
    QString msg = data.value("msg").toString();

    this->writeRecord(type, accountId, groupId, fromUserName+": "+msg);
}

void ChattingRecordUtil::removeFriend(QJsonObject data) {
    QString chattingId = data.value("friendId").toString();
    this->removeRecord(ChattingType::PERSONAL, DataMgr->getAccountId(), chattingId);
}

void ChattingRecordUtil::quitGroup(QJsonObject data) {
    QString chattingId = data.value("groupId").toString();
    this->removeRecord(ChattingType::GROUP, DataMgr->getAccountId(), chattingId);
}

QString ChattingRecordUtil::generateFileName(ChattingType type,
                            QString currentId, QString chattingId) {
    QString fileName;
    if (type == ChattingType::PERSONAL){
        fileName = 'p';
    } else {
        fileName = 'g';
    }
    fileName += '-' + currentId + '-' + chattingId + ".rec";
    return fileName;
}
