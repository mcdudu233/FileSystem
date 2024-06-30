//
// Created by 86138 on 2024/6/23.
//

#ifndef FILESYSTEM_LOGIN_H
#define FILESYSTEM_LOGIN_H

#include "filesystem.h"
#include <QDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
    class login;
}
QT_END_NAMESPACE

class login : public QDialog {
    Q_OBJECT

public:
    explicit login(filesystem *fs, QWidget *parent = nullptr);
    ~login() override;

private:
    Ui::login *ui;
    filesystem *fs;

private slots:
    void onLoginButtonClicked();
};

#endif//FILESYSTEM_LOGIN_H
