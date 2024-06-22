//
// Created by dudu233 on 2024/6/17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include "mainwindow.h"
#include "../form/ui_mainwindow.h"
#include <QMessageBox>
#include <QStatusBar>
// 修改相应的模块名
mainwindow::mainwindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainwindow) {
    ui->setupUi(this);
    connect(ui->treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onCurrentItemChanged(const QModelIndex &)));
    connect(ui->searchEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onSearchTextChanged(const QString &)));
    // 将磁盘容量和剩余磁盘容量标签添加到状态栏
    statusBar()->addPermanentWidget(ui->diskCapacityLabel);
    statusBar()->addPermanentWidget(ui->remainingDiskCapacityLabel);
    // 创建并设置文件列表视图
    fileListView = new FileListView(this);
    setCentralWidget(fileListView);
}

mainwindow::~mainwindow() {
    delete ui;
}

void mainwindow::onFileDoubleClicked(const QModelIndex &index) {
    // 文件被双击，可以在这里处理
}

void mainwindow::onFileRightClicked(const QModelIndex &index) {
    // 文件被右键点击，可以在这里处理
}

void mainwindow::onSearchTextChanged(const QString &text) {
    // 搜索文本发生变化，可以在这里处理搜索逻辑
}


// 显示文件管理系统窗口的槽函数
void mainwindow::showFileManager() {
    // 如果MainWindow是隐藏的，显示它
    if (!isVisible()) {
        showNormal(); // 显示窗口
        raise(); // 使窗口在所有窗口中位于最顶层
    }
}
// 显示登录失败消息的槽函数
void mainwindow::showLoginFailedMessage() {
    QMessageBox::critical(this, "登录失败", "用户名或密码错误。");
}
// 磁盘容量更新函数
void mainwindow::updateDiskCapacity() {
    // 获取磁盘的总容量和剩余容量
    // 更新界面上的标签
}

// 当前项改变时的槽函数
void mainwindow::onCurrentItemChanged(const QModelIndex &current, const QModelIndex &previous) {
    if (current.isValid()) {
        // 更新磁盘容量信息
        updateDiskCapacity();
    }
}