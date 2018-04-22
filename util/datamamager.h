#ifndef DATAMAMAGER_H
#define DATAMAMAGER_H

#include <QObject>
#include <QJsonObject>

#define DataMgr DataMamager::getInstance()

enum ChattingType{
    NONE = 0,
    PERSONAL,
    GROUP
};

struct CurrentChatting{
    CurrentChatting(){
        this->id = "";
        this->type = ChattingType::NONE;
    }
    QString id;
    ChattingType type;
};

class DataMamager: public QObject
{
    Q_OBJECT
public:
    static DataMamager* getInstance(){
        static auto _instance = new DataMamager();
        return _instance;
    }

    void init();

    void setUserData(QString id, QString name);
    void setCurrentChatting(QString id, ChattingType type);

    QString getUserName(){
        return userName;
    }

    QString getAccountId(){
        return accountId;
    }

    CurrentChatting getCurrentChatting() {
        return currentChatting;
    }

public slots:
    void logined(QJsonObject obj);

private:
    DataMamager();

    QString accountId;
    QString userName;
    CurrentChatting currentChatting;
};

#endif // DATAMAMAGER_H
