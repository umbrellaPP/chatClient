#ifndef DIALOGMGR_H
#define DIALOGMGR_H

#include <QObject>
#include <QJsonObject>
#include <login.h>
#include <maindialog.h>
#include <addfriend.h>

#define DialogMgr DialogManager::getInstance()

class DialogManager: public QObject
{
    Q_OBJECT
public:
    static DialogManager* getInstance(){
        static auto _instance = new DialogManager();
        return _instance;
    }

    void showLogin();
    void showMain();
    void showAddFriend();

public slots:
    void registered(QJsonObject data);
    void logined(QJsonObject data);

private:
    DialogManager();

    Login *login;
    MainDialog *mainDlg;
    AddFriend *addFriend;
};

#endif // DIALOGMGR_H
