#include "addfriend.h"
#include "ui_addfriend.h"
#include "util/datamamager.h"
#include <QDebug>
#include <QMessageBox>

AddFriend::AddFriend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFriend)
{
    ui->setupUi(this);
    this->setModal(true);

    connect(ui->lineEditId, SIGNAL(textChanged(QString)), this, SLOT(onIdChanged()));
    connect(Net->handler, SIGNAL(userFound(QJsonObject)), this, SLOT(userFound(QJsonObject)));
    connect(Net->handler, SIGNAL(userNotFound()), this, SLOT(userNotFound()));
    connect(Net->handler, SIGNAL(addFriendResult(QJsonObject)), this, SLOT(addFriendResult(QJsonObject)));
}

AddFriend::~AddFriend()
{
    delete ui;
}

AddFriend::showFound(QString userName) {
    ui->labelNickname->setText("昵称: " + userName);
    ui->labelNickname->show();
    ui->btnAdd->show();
}

AddFriend::showFoundFailure() {
    ui->labelNickname->setText("查无此用户");
    ui->labelNickname->show();
    ui->btnAdd->hide();
}

AddFriend::hideFound() {
    ui->labelNickname->hide();
    ui->btnAdd->hide();
}

AddFriend::showRequestSent() {
    ui->labelNickname->setText("已经发送好友请求");
    ui->labelNickname->show();
    ui->btnAdd->hide();
}

void AddFriend::showEvent(QShowEvent *event) {
    this->hideFound();
}

void AddFriend::on_btnSelect_clicked() {
    QString accountId = ui->lineEditId->text();
    QJsonObject obj;
    obj.insert("accountId", accountId);

    Net->sendPackage(CodeNS::C2S_FIND_FRIEND, obj);
}

void AddFriend::onIdChanged() {
    this->hideFound();
}

void AddFriend::userFound(QJsonObject obj) {
    QString userName = obj.value("userName").toString();
    this->showFound(userName);
}

void AddFriend::userNotFound() {
    this->showFoundFailure();
}

void AddFriend::addFriendResult(QJsonObject data) {
    double result = data.value("result").toDouble();
    if (result == 0) {
        QMessageBox::information(nullptr, "添加成功", "添加好友成功");
    }else if (result == 1) {
        QMessageBox::information(nullptr, "添加失败", "已经是好友关系");
    }else if (result == 2) {
        QMessageBox::information(nullptr, "添加失败", "不能添加自己为好友");
    }
}

void AddFriend::on_btnAdd_clicked() {
    QJsonObject package;
    QString accountId = DataMgr->getAccountId();
    QString friendId = ui->lineEditId->text();
    package.insert("accountId", accountId);
    package.insert("friendId", friendId);
    Net->sendPackage(CodeNS::C2S_ADD_FRIEND, package);

    this->showRequestSent();
}
