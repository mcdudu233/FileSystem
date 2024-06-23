//
// Created by 86138 on 2024/6/23.
//

#ifndef FILESYSTEM_LOGIN_H
#define FILESYSTEM_LOGIN_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui {
    class login;
}
QT_END_NAMESPACE

class login : public QDialog {
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login() override;

private:
    Ui::login *ui;

private slots:
    void onLoginButtonClicked();
    void onRegisterLinkClicked();
    void onForgotPasswordLinkClicked();
signals:
    void loginSuccess();// 登录成功的信号
    void loginFailed(); // 登录失败信号;
};

#endif//FILESYSTEM_LOGIN_H
