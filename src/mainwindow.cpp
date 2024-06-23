//
// Created by dudu233 on 2024/6/17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include "mainwindow.h"
#include "../form/ui_mainwindow.h"

// 修改相应的模块名
mainwindow::mainwindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainwindow) {
    ui->setupUi(this);
    connect(ui->treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onCurrentItemChanged(const QModelIndex &)));
    connect(ui->searchEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onSearchTextChanged(const QString &)));

    const vector<fs::path> &fss = searchFileSystem();
    if (!fss.empty()) {
        // 如果当前文件下有文件系统 则询问是否打开第一个
        string fs_name = fss[0].filename().string();
        QMessageBox::StandardButton reply =
                QMessageBox::question(this,
                                      "检测到文件系统",
                                      QString::fromStdString("当前文件夹下存在文件系统：" + fs_name + "，是否要打开？"),
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            openFileSystem(QString::fromStdString(fs_name).replace(DATA_SUFFIX, ""));
        } else {
            closeFileSystem();
        }
    } else {
        closeFileSystem();
    }
}

mainwindow::~mainwindow() {
    delete ui;
}

void mainwindow::onFileDoubleClicked(const QModelIndex &index) {
    // 文件被双击，可以在这里处理
}

void mainwindow::onFileRightClicked(const QModelIndex &index) {
    // 文件被右键点击，可以在这里处理
    // 创建并设置文件列表视图
    fileListView = new FileListView(this);
    setCentralWidget(fileListView);
}

void mainwindow::onSearchTextChanged(const QString &text) {
    // 搜索文本发生变化，可以在这里处理搜索逻辑
}


// 显示文件管理系统窗口的槽函数
void mainwindow::showFileManager() {
    // 如果MainWindow是隐藏的，显示它
    if (!isVisible()) {
        showNormal();// 显示窗口
        raise();     // 使窗口在所有窗口中位于最顶层
    }
}

//显示登录失败消息的槽函数
void mainwindow::showLoginFailedMessage() {
    QMessageBox::critical(this, "登录失败", "用户名或密码错误。");
}

// 磁盘容量更新函数
void mainwindow::updateDiskCapacity() {
    // 获取磁盘的总容量和剩余容量
    // 更新界面上的标签
}

// 当前项改变时的槽函数
void mainwindow::onCurrentItemChanged(const QModelIndex &current) {
    if (current.isValid()) {
        // 更新磁盘容量信息
        updateDiskCapacity();
    }
}

void mainwindow::onFileSelected(const QModelIndex &index) {
}

vector<fs::path> mainwindow::searchFileSystem() {
    // 搜索当前目录下的数据文件
    fs::path current_path = fs::current_path();
    // 用于存储找到的.fs文件的路径
    vector<fs::path> fs_files;
    // 遍历当前目录
    for (const auto &entry: fs::directory_iterator(current_path)) {
        // 检查是否为常规文件且后缀匹配
        if (entry.is_regular_file()) {
            // 获取文件名
            string filename = entry.path().filename().string();
            if (filename.size() >= sizeof(DATA_SUFFIX) - 1 && filename.substr(filename.size() - (sizeof(DATA_SUFFIX) - 1)) == DATA_SUFFIX) {
                fs_files.push_back(entry.path());
            }
        }
    }
    return fs_files;
}

void mainwindow::openFileSystem(QString name) {
    fsx = new filesystem(name.toStdString());
    ui->openButton->setDisabled(true);
    ui->closeButton->setDisabled(false);
}

void mainwindow::closeFileSystem() {
    if (fsx != nullptr) {
        delete fsx;
        fsx = nullptr;
    }
    ui->openButton->setDisabled(false);
    ui->closeButton->setDisabled(true);
}
