#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "define.h"
#include "net/client.h"
#include <QJsonObject>
#include <QMessageBox>

enum Mode {
    LOGIN,
    REGISTER
};

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_btnLogin_clicked();
    void on_btnReg_clicked();
    void on_btnCanel_clicked();

    void connectedToServer();

private:
    void updateUI();
    void enableBtns();

    Mode mode;

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
