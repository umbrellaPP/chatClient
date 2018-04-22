#ifndef CHATTINGRECORDUTIL_H
#define CHATTINGRECORDUTIL_H

#include <QObject>
#include <QJsonArray>
#include "util/datamamager.h"

#define ChattingRecord ChattingRecordUtil::getInstance()

class ChattingRecordUtil : public QObject {
    Q_OBJECT
public:
    static ChattingRecordUtil *getInstance() {
        static auto _instance = new ChattingRecordUtil();
        return _instance;
    }

    void init();
    QJsonArray readRecord(ChattingType type, QString currentId, QString chattingId);
    void writeRecord(ChattingType type, QString currentId, QString chattingId, QString newMsg);

public slots:
    void newMsg(QJsonObject data);
    void sendMsg(QJsonObject data);
    void newGroupMsg(QJsonObject data);

private:
    QString generateFileName(ChattingType type, QString currentId, QString chattingId);
    ChattingRecordUtil(QObject *parent = 0);

signals:

public slots:
};

#endif // CHATTINGRECORDUTIL_H
