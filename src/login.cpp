//
// Created by 86138 on 2024/6/22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_login.h" resolved

#include "login.h"
#include "../form/ui_login.h"


login::login(QWidget *parent) : QWidget(parent), ui(new Ui::login) {
    ui->setupUi(this);
    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    loginButton = new QPushButton("登录", this);
    loginStatusLabel = new QLabel("", this);

    // 设置布局
    mainLayout = new QVBoxLayout(this);
    formLayout = new QHBoxLayout();
    loginFormLayout = new QFormLayout();

    loginFormLayout->addRow("用户名:", usernameEdit);
    loginFormLayout->addRow("密码:", passwordEdit);
    formLayout->addLayout(loginFormLayout);
    formLayout->addWidget(loginButton);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(loginStatusLabel);

    // 设置布局
    setLayout(mainLayout);

    // 连接信号和槽
    connect(loginButton, SIGNAL(clicked()), this, SLOT(onLoginButtonClicked()));

}


login::~login() {
    delete ui;
}
void login::onLoginButtonClicked() {
    // 登录逻辑
    // 这里需要实现用户名和密码的验证
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    if (username == "valid_username" && password == "valid_password") {
        loginStatusLabel->setText("登录成功");
        // 继续登录流程
    } else {
        loginStatusLabel->setText("登录失败");
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