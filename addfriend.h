#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QDialog>
#include <QJsonObject>
#include "net/client.h"

namespace Ui {
class AddFriend;
}

class AddFriend : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriend(QWidget *parent = 0);
    ~AddFriend();

    showFound(QString userName);
    showFoundFailure();
    hideFound();
    showRequestSent();

    void showEvent(QShowEvent *event);

private slots:
    void on_btnSelect_clicked();

    void onIdChanged();
    void userFound(QJsonObject obj);
    void userNotFound();
    void addFriendResult(QJsonObject data);

    void on_btnAdd_clicked();

private:
    Ui::AddFriend *ui;
};

#endif // ADDFRIEND_H
