#include "dialogmgr.h"
#include <QDebug>

DialogManager::DialogManager() {
    login = new Login();
    mainDlg = new MainDialog();
    addFriend = new AddFriend();

    connect(Net->handler, SIGNAL(registered(QJsonObject)), this, SLOT(registered(QJsonObject)));
    connect(Net->handler, SIGNAL(logined(QJsonObject)), this, SLOT(logined(QJsonObject)));
}

void DialogManager::showLogin() {
    login->show();
    mainDlg->hide();
    addFriend->hide();
}

void DialogManager::showMain() {
    login->hide();
    mainDlg->show();
    addFriend->hide();
}

void DialogManager::showAddFriend() {
    addFriend->show();
}

void DialogManager::registered(QJsonObject data) {
    qDebug() << "registered";
}

void DialogManager::logined(QJsonObject data) {
    this->mainDlg->initUserData(data);
    this->showMain();
}
