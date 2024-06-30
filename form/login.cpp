//
// Created by 86138 on 2024/6/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_login.h" resolved

#include "login.h"
#include "ui_login.h"

login::login(filesystem *fs, QWidget *parent) : QDialog(parent), ui(new Ui::login) {
    ui->setupUi(this);
    setWindowTitle("登录");
    // 设置窗口图标
    QIcon icon("./res/main.png");
    if (icon.isNull()) {
        icon.addFile("../res/main.png");
    }
    setWindowIcon(icon);

    // 连接信号和槽
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(onLoginButtonClicked()));

    // 加载所有用户
    this->fs = fs;
    const vector<user> &usrs = fs->usrs();
    for (auto u: usrs) {
        ui->usernameEdit->addItem(u.getName().c_str());
    }
}

login::~login() {
    delete ui;
}

void login::onLoginButtonClicked() {
    // 登录逻辑
    user usr = fs->userbyid(ui->usernameEdit->currentIndex());
    QString password = ui->passwordEdit->text();
    if (usr.checkPassword(password.toStdString())) {
        // 继续登录流程
        QMessageBox::information(this, "登录成功", QString::fromStdString("您以 " + usr.getName() + " 身份成功登陆。请继续操作。"));
        accept();
    } else {
        QMessageBox::critical(this, "登录失败", "用户名或密码错误！");
        reject();
    }
}