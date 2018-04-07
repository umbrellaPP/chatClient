#include "login.h"
#include "ui_login.h"
#include <QCoreApplication>
#include <QEvent>
#include "util/myevent.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    MyEvent *e = new MyEvent(QEvent::Type(Event::Test1));
    QCoreApplication::sendEvent(Net, e);

    this->mode = Mode::LOGIN;
    this->updateUI();
    Net->initWeb();
    connect(Net, SIGNAL(connectedToServer()), this, SLOT(connectedToServer()));
}

Login::~Login() {
    delete ui;
}

void Login::updateUI() {
    bool isLoginMode = (this->mode == Mode::LOGIN);
    ui->btnCanel->setVisible(!isLoginMode);
    ui->btnLogin->setVisible(isLoginMode);

    ui->userName->setVisible(!isLoginMode);
    ui->lineEditUserName->setVisible(!isLoginMode);

    ui->accountId->setVisible(isLoginMode);
    ui->lineEditAccountId->setVisible(isLoginMode);

    ui->passwordConfirm->setVisible(!isLoginMode);
    ui->lineEditPasswordConfirm->setVisible(!isLoginMode);

    this->resize(this->sizeHint());
}

void Login::on_btnLogin_clicked() {
    if (!Net->isConnected()){
        qDebug() << "未连接服务器";
        return;
    }
    QString accountId = ui->lineEditAccountId->text();
    QString password = ui->lineEditPassword->text();

    if (accountId.isEmpty()) {
        ui->lineEditAccountId->setPlaceholderText("账号不能为空");
        return;
    }
    if (password.isEmpty()) {
        ui->lineEditPassword->setPlaceholderText("密码不能为空");
        return;
    }

    QJsonObject obj;
    obj.insert("accountId", accountId);
    obj.insert("password", password);
    Net->sendPackage(Code::C2S_LOGIN, obj);
}

void Login::on_btnReg_clicked() {
    if (this->mode == Mode::LOGIN) {
        this->mode = Mode::REGISTER;
        this->updateUI();
    } else if (this->mode == Mode::REGISTER) {
        if (!Net->isConnected()) {
            qDebug() << "未连接服务器";
            return;
        }
        QString userName = ui->lineEditUserName->text();
        if (userName.isEmpty()) {
            ui->lineEditUserName->setPlaceholderText("用户名不能为空");
            return;
        }
        QString password = ui->lineEditPassword->text();
        QString passwordComfrim = ui->lineEditPasswordConfirm->text();
        if (password.isEmpty()) {
            ui->lineEditPassword->setPlaceholderText("密码不能为空");
            return;
        } else if (password != passwordComfrim) {
            QMessageBox::information(this, "错误", "两次密码不一致");
            return;
        }

        QJsonObject obj;
        obj.insert("userName", userName);
        obj.insert("password", password);
        Net->sendPackage(Code::C2S_REGISTER, obj);
    }
}

void Login::on_btnCanel_clicked() {
    this->mode = Mode::LOGIN;
    this->updateUI();
}

void Login::connectedToServer() {
    this->enableBtns();
}

void Login::enableBtns() {
    ui->btnLogin->setEnabled(true);
    ui->btnReg->setEnabled(true);
}
