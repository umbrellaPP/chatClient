#include "maindialog.h"
#include "ui_maindialog.h"
#include "util/dialogmgr.h"
#include <QMessageBox>
#include <QInputDialog>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);
    ui->spliterMain->setStretchFactor(0, 1);
    ui->spliterMain->setStretchFactor(1, 3);
    ui->splitterLeft->setStretchFactor(0, 1);
    ui->splitterLeft->setStretchFactor(1, 1);
    ui->splitterRight->setStretchFactor(0, 3);
    ui->splitterRight->setStretchFactor(1, 1);

    ui->btnDelFriend->setEnabled(false);
    ui->btnQuitGroup->setEnabled(false);

    connect(Net->handler, SIGNAL(updateFriendList(QJsonObject)),
            this, SLOT(updateFriendList(QJsonObject)));
    connect(Net->handler, SIGNAL(removeFriend(QJsonObject)),
            this, SLOT(removeFriend(QJsonObject)));
    connect(ui->listWidgetFriends, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(friendListItemClicked(QListWidgetItem*)));
    connect(ui->listWidgetGroups, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(groupListItemClicked(QListWidgetItem*)));
    connect(Net->handler, SIGNAL(sendMsg(QJsonObject)),
            this, SLOT(sendMsg(QJsonObject)));
    connect(Net->handler, SIGNAL(newMsg(QJsonObject)), this, SLOT(newMsg(QJsonObject)));

    connect(Net->handler, SIGNAL(createGroup(QJsonObject)), this, SLOT(createGroup(QJsonObject)));
    connect(Net->handler, SIGNAL(updateGroupList(QJsonObject)), this, SLOT(updateGroupList(QJsonObject)));
    connect(Net->handler, SIGNAL(findGroup(QJsonObject)), this, SLOT(findGroup(QJsonObject)));
    connect(Net->handler, SIGNAL(joinGroup(QJsonObject)), this, SLOT(joinGroup(QJsonObject)));
    connect(Net->handler, SIGNAL(updateMembers(QJsonObject)), this, SLOT(updateMembers(QJsonObject)));
    connect(Net->handler, SIGNAL(newGroupMsg(QJsonObject)), this, SLOT(newGroupMsg(QJsonObject)));
    connect(Net->handler, SIGNAL(quitGroup(QJsonObject)), this, SLOT(quitGroup(QJsonObject)));
}

MainDialog::~MainDialog() {
    delete ui;
}

void MainDialog::initUserData(QJsonObject data) {
    QString userName = data.value("userName").toString();
    QString accountId = data.value("accountId").toString();

    this->setWindowTitle(userName + " - " + accountId);
}

void MainDialog::redirectCurrentChatting(ChattingType type, QString id) {
    CurrentChatting cc = DataMgr->getCurrentChatting();
    if (cc.type == type && cc.id == id) {
        for (int i = 0; i < ui->listWidgetFriends->count(); ++i) {
            auto item = ui->listWidgetFriends->itemAt(i, 0);
            auto accountId = item->toolTip();
            if (accountId != id) {
                this->friendListItemClicked(item);
                return;
            }
        }
        for (int i = 0; i < ui->listWidgetGroups->count(); ++i) {
            auto item = ui->listWidgetGroups->itemAt(i, 0);
            auto groupId = item->toolTip();
            if (groupId != id) {
                this->groupListItemClicked(item);
                return;
            }
        }
        DataMgr->setCurrentChatting("", ChattingType::NONE);
        ui->labelCurrentChatting->setText("当前聊天: 无");
        ui->listWidgetChat->clear();
        ui->btnDelFriend->setEnabled(false);
        ui->btnQuitGroup->setEnabled(false);
    }
}

void MainDialog::on_btnAddFriend_clicked() {
    DialogMgr->showAddFriend();
}

void MainDialog::removeFriend(QJsonObject data) {
    double result = data.value("result").toDouble();
    if (result == 0) {
        QString friendUserName = data.value("friendUserName").toString();
        QString friendId = data.value("friendId").toString();
        QMessageBox::information(this, "删除成功", QString("好友%1 (%2)已经删除")
                                 .arg(friendId).arg(friendUserName));

        this->redirectCurrentChatting(ChattingType::PERSONAL, friendId);
    } else if (result == 1) {
        QMessageBox::information(this, "删除失败", "好友不存在");
    }
}

void MainDialog::updateFriendList(QJsonObject data) {
    QString friendIds = data.value("friendIds").toString();
    QString friendUserNames = data.value("friendUserNames").toString();

    QStringList friendIdList = friendIds.split(',');
    QStringList friendUserNameList = friendUserNames.split(',');
    ui->listWidgetFriends->clear();
    if (friendIds.isEmpty()) {
        return ;
    }
    for (int i = 0; i < friendIdList.length(); ++i){
        auto item = new QListWidgetItem(friendUserNameList[i]);
        item->setToolTip(friendIdList[i]);
        ui->listWidgetFriends->addItem(item);
    }

    if (ui->listWidgetFriends->count() > 0){
        ui->listWidgetFriends->setCurrentRow(0);
        this->friendListItemClicked(ui->listWidgetFriends->item(0));
    }
}

void MainDialog::friendListItemClicked(QListWidgetItem *item) {
    QString userName = item->text();
    QString accountId = item->toolTip();
    CurrentChatting cc = DataMgr->getCurrentChatting();
    if (cc.type == ChattingType::PERSONAL
            && accountId == cc.id){
        return ;
    }
    DataMgr->setCurrentChatting(accountId, ChattingType::PERSONAL);
    QJsonArray rec = ChattingRecord->readRecord(ChattingType::PERSONAL, DataMgr->getAccountId(), accountId);
    ui->listWidgetChat->clear();
    for (QJsonValue value: rec) {
        ui->listWidgetChat->addItem(value.toString());
    }

    item->setTextColor(Qt::black);
    ui->listWidgetChat->setCurrentRow(ui->listWidgetChat->count()-1);
    ui->listWidgetFriends->setCurrentItem(item);
    ui->listWidgetGroups->setCurrentRow(-1);
    ui->btnDelFriend->setEnabled(true);
    ui->btnQuitGroup->setEnabled(false);
    ui->labelCurrentChatting->setText(QString("当前聊天: %1 - %2").arg(userName).arg(accountId));
}

void MainDialog::groupListItemClicked(QListWidgetItem *item) {
    QString groupId = item->toolTip();
    CurrentChatting cc = DataMgr->getCurrentChatting();
    if (cc.type == ChattingType::GROUP
            && groupId == cc.id){
        return ;
    }
    DataMgr->setCurrentChatting(groupId, ChattingType::GROUP);

    QJsonArray rec = ChattingRecord->readRecord(ChattingType::GROUP, DataMgr->getAccountId(), groupId);
    ui->listWidgetChat->clear();
    for (QJsonValue value: rec) {
        ui->listWidgetChat->addItem(value.toString());
    }
    item->setTextColor(Qt::black);
    ui->listWidgetChat->setCurrentRow(ui->listWidgetChat->count()-1);
    ui->listWidgetGroups->setCurrentItem(item);
    ui->listWidgetFriends->setCurrentRow(-1);
    ui->btnDelFriend->setEnabled(false);
    ui->btnQuitGroup->setEnabled(true);

    QJsonObject package;
    package.insert("groupId", groupId);
    Net->sendPackage(CodeNS::C2S_UPDATE_MEMBERS, package);
}

void MainDialog::on_pushButtonSend_clicked() {
    QString msg = ui->textEditChat->toPlainText();
    CurrentChatting cc = DataMgr->getCurrentChatting();
    if (msg.isEmpty()){
        QMessageBox::information(this, "发送失败", "内容不能为空");
    } else if (cc.type == ChattingType::PERSONAL){
        QString fromId = DataMgr->getAccountId();
        QString toId = cc.id;
        if (toId.isEmpty()) {
            QMessageBox::information(this, "发送失败", "请选择一个聊天对象");
            return ;
        }
        QJsonObject package;
        package.insert("fromId", fromId);
        package.insert("toId", toId);
        package.insert("msg", msg);
        Net->sendPackage(CodeNS::C2S_SEND_MSG, package);
    } else if (cc.type == ChattingType::GROUP) {
        QString accountId = DataMgr->getAccountId();
        QString groupId = cc.id;
        if (groupId.isEmpty()) {
            QMessageBox::information(this, "发送失败", "请选择一个聊天对象");
            return ;
        }
        QJsonObject package;
        package.insert("accountId", accountId);
        package.insert("groupId", groupId);
        package.insert("msg", msg);
        qDebug() << package;
        Net->sendPackage(CodeNS::C2S_SEND_GROUP_MSG, package);
    }

    ui->textEditChat->clear();
}

void MainDialog::sendMsg(QJsonObject data) {
    double result = data.value("result").toDouble();
//    QString fromId = data.value("fromId").toString();
//    QString toId = data.value("toId").toString();
    QString fromUserName = data.value("fromUserName").toString();
    QString msg = data.value("msg").toString();

    if (result == 0) {
        ui->listWidgetChat->addItem(fromUserName + ": " + msg);
    } else if (result == 1) {
        QMessageBox::information(this, "发送失败", "对方不在线");
    }
}

void MainDialog::newMsg(QJsonObject data) {
    QString toId = data.value("toId").toString();
    QString fromId = data.value("fromId").toString();
    QString fromUserName = data.value("fromUserName").toString();
    QString msg = data.value("msg").toString();
    CurrentChatting cc = DataMgr->getCurrentChatting();

    if (cc.type == ChattingType::PERSONAL && fromId == cc.id) {
        ui->listWidgetChat->addItem(fromUserName + ": " + msg);
    } else {
        this->announceNewMsg(ChattingType::PERSONAL, fromId);
    }
}

void MainDialog::createGroup(QJsonObject data) {
    double result = data.value("result").toDouble();
    if (result == 0) {
        QString groupName = data.value("groupName").toString();
        QString groupId = data.value("groupId").toString();
        QMessageBox::information(this, "创建成功",
                     QString("群%1(%2)创建成功，将群号分享给你的好友吧！").arg(groupId).arg(groupName));
    } else if (result == 1) {
        QMessageBox::information(this, "创建失败", "群创建失败，请稍后重试");
    }
}

void MainDialog::updateGroupList(QJsonObject data) {
    QString groups = data.value("groups").toString();
    QString groupNames = data.value("groupNames").toString();

    QStringList groupList = groups.split(',');
    QStringList groupNameList = groupNames.split(',');

    ui->listWidgetGroups->clear();
    if (groups.isEmpty()) {
        return ;
    }
    for (int i = 0; i < groupList.count(); ++i) {
        auto groupId = groupList.at(i);
        auto groupName = groupNameList.at(i);
        QListWidgetItem *item = new QListWidgetItem(groupName);
        item->setToolTip(groupId);
        ui->listWidgetGroups->addItem(item);
    }
}

void MainDialog::findGroup(QJsonObject data) {
    qDebug() << data;
    double result = data.value("result").toDouble();
    if (result == 0) {
        QString groupId = data.value("groupId").toString();
        QString groupName = data.value("groupName").toString();
        int btn = QMessageBox::information(this, "查找成功", QString("查到群%1 (%2), 是否加入此群?")
                                           .arg(groupId).arg(groupName),
                                           QMessageBox::Ok | QMessageBox::Cancel);
        if (btn == QMessageBox::Ok) {
            QJsonObject package;
            package.insert("accountId", DataMgr->getAccountId());
            package.insert("groupId", groupId);
            Net->sendPackage(CodeNS::C2S_JOIN_GROUP, package);
        }
    } else if (result == 1) {
        QMessageBox::information(this, "查找失败", "查无此群");
    }
}

void MainDialog::joinGroup(QJsonObject data) {
    double result = data.value("result").toDouble();
    if (result == 0) {
        // 成功
        QString groupId = data.value("groupId").toString();
        QString groupName = data.value("groupName").toString();
        QMessageBox::information(this, "加入成功", QString("你已成功加入群%1 (%2)").arg(groupId).arg(groupName));
    } else if (result == 1) {
        // 不能重复加入
        QMessageBox::information(this, "加入失败", "不能重复加入");
    }
}

void MainDialog::updateMembers(QJsonObject data) {
    QString groupId = data.value("groupId").toString();
    QString groupName = data.value("groupName").toString();
    QString memberUserNames = data.value("memberUserNames").toString();
    auto cc = DataMgr->getCurrentChatting();
    if (cc.type == ChattingType::GROUP && cc.id == groupId) {
        ui->labelCurrentChatting->setText(QString("当前聊天: 群 %1 - %2\n群成员: %3")
                                          .arg(groupName).arg(groupId).arg(memberUserNames));
    }
}

void MainDialog::newGroupMsg(QJsonObject data) {
    QString fromUserName = data.value("fromUserName").toString();
    QString groupId = data.value("groupId").toString();
    QString msg = data.value("msg").toString();
    CurrentChatting cc = DataMgr->getCurrentChatting();

    if (cc.type == ChattingType::GROUP && cc.id == groupId) {
        ui->listWidgetChat->addItem(fromUserName + ": " + msg);
    } else {
        announceNewMsg(ChattingType::GROUP, groupId);
    }
}

void MainDialog::quitGroup(QJsonObject data) {
    double result = data.value("result").toDouble();
    if (result == 0) {
        // 解散
        QString groupId = data.value("groupId").toString();
        QString groupName = data.value("groupName").toString();
        QMessageBox::information(this, "退群成功",
                                 QString("群%1 (%2)已经解散").arg(groupId).arg(groupName));

        this->redirectCurrentChatting(ChattingType::GROUP, groupId);
    } else if (result == 1) {
        // 退群
        QString groupId = data.value("groupId").toString();
        QString groupName = data.value("groupName").toString();
        QMessageBox::information(this, "退群成功",
                                 QString("你已经退出群%1 (%2)").arg(groupId).arg(groupName));

        this->redirectCurrentChatting(ChattingType::GROUP, groupId);
    }
}

void MainDialog::announceNewMsg(ChattingType chattingType, QString id) {
    QListWidget *listWidget =
            chattingType==ChattingType::PERSONAL? ui->listWidgetFriends: ui->listWidgetGroups;
    for (int i = 0; i < listWidget->count(); ++i) {
        auto item = listWidget->item(i);
        if (item->toolTip() == id){
            item->setTextColor(Qt::red);
            return;
        }
    }
}

void MainDialog::on_btnCreateGroup_clicked() {
    bool ok;
    QString groupName = QInputDialog::getText(this, "建群", "请输入群昵称", QLineEdit::Normal, "", &ok);
    if (ok) {
        if (groupName.isEmpty()) {
            QMessageBox::information(this, "输入错误", "群昵称不能为空");
        } else {
            // 请求建群
            QJsonObject package;
            package.insert("groupName", groupName);
            package.insert("accountId", DataMgr->getAccountId());
            Net->sendPackage(CodeNS::C2S_CREATE_GROUP, package);
        }
    }
}

void MainDialog::on_btnJoinGroup_clicked() {
    bool ok;
    QString groupId = QInputDialog::getText(this, "查找群", "请输入群ID", QLineEdit::Normal, "", &ok);
    if (ok) {
        QJsonObject package;
        package.insert("groupId", groupId);
        Net->sendPackage(CodeNS::C2S_FIND_GROUP, package);
    }
}

void MainDialog::on_btnDelFriend_clicked() {
    QString accountId = DataMgr->getAccountId();
    QString friendId = ui->listWidgetFriends->currentItem()->toolTip();
    QJsonObject package;

    package.insert("accountId", accountId);
    package.insert("friendId", friendId);
    Net->sendPackage(CodeNS::C2S_REMOVE_FRIEND, package);
}

void MainDialog::on_btnQuitGroup_clicked() {
    QString accountId = DataMgr->getAccountId();
    QString groupId = ui->listWidgetGroups->currentItem()->toolTip();
    QJsonObject package;

    package.insert("accountId", accountId);
    package.insert("groupId", groupId);
    Net->sendPackage(CodeNS::C2S_QUIT_GROUP, package);
}
