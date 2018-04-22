#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include <QListWidgetItem>
#include "util/datamamager.h"
#include "util/chattingrecordutil.h"

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

    void initUserData(QJsonObject data);
    void redirectCurrentChatting(ChattingType type, QString id);

private slots:
    void on_btnAddFriend_clicked();
    void removeFriend(QJsonObject data);
    void updateFriendList(QJsonObject data);
    void on_pushButtonSend_clicked();
    void sendMsg(QJsonObject data);
    void newMsg(QJsonObject data);

    void createGroup(QJsonObject data);
    void updateGroupList(QJsonObject data);
    void findGroup(QJsonObject data);
    void joinGroup(QJsonObject data);
    void updateMembers(QJsonObject data);
    void newGroupMsg(QJsonObject data);
    void quitGroup(QJsonObject data);

    void friendListItemClicked(QListWidgetItem *item);
    void groupListItemClicked(QListWidgetItem *item);

    void on_btnCreateGroup_clicked();
    void on_btnJoinGroup_clicked();

    void on_btnDelFriend_clicked();

    void on_btnQuitGroup_clicked();

private:
    Ui::MainDialog *ui;

    void announceNewMsg(ChattingType chattingType, QString id);
};

#endif // MAINDIALOG_H
