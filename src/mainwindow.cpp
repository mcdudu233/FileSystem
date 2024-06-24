//
// Created by dudu233 on 2024/6/17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include "mainwindow.h"
#include "../form/ui_mainwindow.h"
#include <QDir>
#include <QFileSystemModel>

// 修改相应的模块名
mainwindow::mainwindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainwindow) {
    ui->setupUi(this);
    setWindowTitle("文件系统管理器");

    connect(ui->treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onCurrentItemChanged(const QModelIndex &)));
    connect(ui->searchEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onSearchTextChanged(const QString &)));
    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(openButtonCliked()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(closeButtonClicked()));
    connect(ui->reformatButton, SIGNAL(clicked()), this, SLOT(reformatButtonClicked()));
    connect(ui->actionNewUser, SIGNAL(triggered(bool)), this, SLOT(newUser()));
    connect(ui->actionNewFile, SIGNAL(triggered(bool)), this, SLOT(newFile()));
    connect(ui->actionNewsystem, SIGNAL(triggered(bool)), this, SLOT(newSystem()));
    connect(ui->actionDeleteUser, SIGNAL(triggered(bool)), this, SLOT(deleteUser()));
    connect(ui->actionDeleteFile, SIGNAL(triggered(bool)), this, SLOT(deleteFile()));
    connect(ui->actionDeleteSystem, SIGNAL(triggered(bool)), this, SLOT(deleteSystem()));
    connect(ui->actionExitSystem, SIGNAL(triggered(bool)), this, SLOT(exitSystem()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(about()));


    //    const vector<fs::path> &fss = searchFileSystem();
    //    if (!fss.empty()) {
    //        // 如果当前文件下有文件系统 则询问是否打开第一个
    //        string fs_name = fss[0].filename().string();
    //        QMessageBox::StandardButton reply =
    //                QMessageBox::question(this,
    //                                      "检测到文件系统",
    //                                      QString::fromStdString("当前文件夹下存在文件系统：" + fs_name + "，是否要打开？"),
    //                                      QMessageBox::Yes | QMessageBox::No,
    //                                      QMessageBox::No);
    //        if (reply == QMessageBox::Yes) {
    //            openFileSystem(QString::fromStdString(fs_name).replace(DATA_SUFFIX, ""));
    //        } else {
    //            closeFileSystem();
    //        }
    //    } else {
    //        closeFileSystem();
    //    }
    closeFileSystem();
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
// 打开文件系统
void mainwindow::openButtonCliked() {
    const vector<fs::path> &fss = searchFileSystem();
    if (fss.empty()) {
        QMessageBox::StandardButton reply = QMessageBox::critical(this,
                                                                  "错误",
                                                                  "没有找到文件系统！是否立即格式化一个？",
                                                                  QMessageBox::Yes | QMessageBox::No,
                                                                  QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            reformat rf;
            if (rf.exec() == QDialog::Accepted) {
                openFileSystem(rf.getFileSystem());
            } else {
                return;
            }
        } else {
            return;
        }
    } else {
        if (fss.size() == 1) {
            string fs_name = fss[0].filename().string();
            //        QMessageBox::information(this,
            //                                 "成功",
            //                                 QString::fromStdString("找到文件系统数据：" + fs_name + "。\n成功打开文件系统！"));
            openFileSystem(QString::fromStdString(fs_name));
        } else {
            DriveSelectionDialog dsd;
            dsd.setPath(fss);
            if (dsd.exec() == QDialog::Accepted) {
                openFileSystem(QString::fromStdString(dsd.getSelected()));
            } else {
                return;
            }
        }
    }
}

// 关闭文件系统
void mainwindow::closeButtonClicked() {
    closeFileSystem();
}

// 格式化
void mainwindow::reformatButtonClicked() {
    reformat rf;
    if (rf.exec() == QDialog::Accepted) {
        delete rf.getFileSystem();
    }
}

void mainwindow::onFileSelected(const QModelIndex &index) {
}

// 新建用户
void mainwindow::newUser() {

};

// 删除用户
void mainwindow::deleteUser() {

};

// 新开文件
void mainwindow::newFile() {

};

// 删除文件
void mainwindow::deleteFile() {

};

//关于
void mainwindow::about() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("关于");
    msgBox.setText("尊敬的老师：\n"
                   "\n"
                   "在这个学期的学习过程中，我深知自己的不足之处，但我也在不断努力，力求在各个方面取得更好的成绩。我明白，期末考试不仅是对我们一个学期学习成果的检验，更是对我们综合素质的一次考核。\n"
                   "\n"
                   "在此，我想向您表达我的诚意和决心。我将以严谨的态度对待每一门考试，认真复习，确保自己在考试中发挥出最佳水平。同时，我也将秉持诚信的原则，严格遵守考试纪律，坚决杜绝任何作弊行为。\n"
                   "\n"
                   "我深知，成绩的取得离不开老师的辛勤付出。因此，在接下来的时间里，我将更加珍惜老师的教诲，努力学习，不断提高自己的综合素质。我相信，在老师的悉心教导下，我一定能够在期末考试中取得优异的成绩。\n"
                   "\n"
                   "最后，我衷心感谢老师一个学期以来的关心和教诲。我将以优异的期末成绩回报老师的辛勤付出，并为班级的荣誉贡献自己的一份力量。\n"
                   "\n"
                   "谢谢老师！\n"
                   "\n"
                   "此致 敬礼！");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
};

// 新建系统
void mainwindow::newSystem() {
}

// 删除系统
void mainwindow::deleteSystem() {
}

// 退出系统
void mainwindow::exitSystem() {
}


/* 工具类 */
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

// 打开文件系统
void mainwindow::openFileSystem(QString name) {
    openFileSystem(new filesystem(name.replace(DATA_SUFFIX, "").toStdString()));
}
void mainwindow::openFileSystem(filesystem *fs) {
    fsX = fs;
    ui->openButton->setDisabled(true);
    ui->closeButton->setDisabled(false);
    displayFileSystem();
}

// 关闭文件系统
void mainwindow::closeFileSystem() {
    if (fsX != nullptr) {
        delete fsX;
        fsX = nullptr;
    }
    if (fsModel != nullptr) {
        delete fsModel;
        fsModel = nullptr;
    }
    ui->openButton->setDisabled(false);
    ui->closeButton->setDisabled(true);
}

void mainwindow::displayFileSystem() {
    if (fsX != nullptr) {
        // 创建文件系统模型
        if (fsModel == nullptr) {
            fsModel = new FileSystemModel(fsX->getTree(), this);
            fsModel->setHeaderData(0, Qt::Horizontal, "名称");
            fsModel->setHeaderData(1, Qt::Horizontal, "大小");
            fsModel->setHeaderData(2, Qt::Horizontal, "类型");
            fsModel->setHeaderData(3, Qt::Horizontal, "最后修改时间");
        }
        ui->treeView->setModel(fsModel);
    }
}
