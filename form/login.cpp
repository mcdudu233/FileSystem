//
// Created by 86138 on 2024/6/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_login.h" resolved

#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) : QDialog(parent), ui(new Ui::login) {
    ui->setupUi(this);
    setWindowTitle("登录");

    // 连接信号和槽
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(onLoginButtonClicked()));
}

login::~login() {
    delete ui;
}

void login::onLoginButtonClicked() {
    // 登录逻辑
    // 这里需要实现用户名和密码的验证
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();
    if (username == "valid_username" && password == "valid_password") {
        // 继续登录流程
        QMessageBox::information(this, "登录成功", "请继续操作。");
        accept();
    } else {
        QMessageBox::critical(this, "登录失败", "用户名或密码错误！");
        reject();
    }
}

void login::onRegisterLinkClicked() {
    // 注册逻辑
    // 这里需要打开注册界面
}

void login::onForgotPasswordLinkClicked() {
    // 忘记密码逻辑
    // 这里需要打开忘记密码界面
}