#include "QApplication"
#include "filesystem.h"
#include "mainwindow.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);



    // 用户登录逻辑
    //connect(loginWindow, &LoginWindow::loginSuccess, this, &mainwindow::show);
    //connect(loginWindow, &LoginWindow::loginFailed, this, &mainwindow::showLoginFailedMessage);

    return app.exec(); // 启动应用程序的事件循环
}

