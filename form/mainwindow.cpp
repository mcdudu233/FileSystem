//
// Created by dudu233 on 2024/6/17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include "mainwindow.h"
#include "ui_mainwindow.h"

// 修改相应的模块名
mainwindow::mainwindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainwindow) {
    ui->setupUi(this);
    setWindowTitle("文件系统管理器");

    connect(ui->treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onCurrentItemChanged(const QModelIndex &)));
    connect(ui->searchEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onSearchTextChanged(const QString &)));
    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(openSystem()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(closeSystem()));
    connect(ui->reformatButton, SIGNAL(clicked()), this, SLOT(reformatSystem()));
    connect(ui->actionNewUser, SIGNAL(triggered(bool)), this, SLOT(newUser()));
    connect(ui->actionNewFile, SIGNAL(triggered(bool)), this, SLOT(newFile()));
    connect(ui->actionNewsystem, SIGNAL(triggered(bool)), this, SLOT(reformatSystem()));
    connect(ui->actionDeleteUser, SIGNAL(triggered(bool)), this, SLOT(deleteUser()));
    connect(ui->actionDeleteFile, SIGNAL(triggered(bool)), this, SLOT(deleteFile()));
    connect(ui->actionOpenSystem, SIGNAL(triggered(bool)), this, SLOT(openSystem()));
    connect(ui->actionCloseSystem, SIGNAL(triggered(bool)), this, SLOT(closeSystem()));
    connect(ui->actionExitSystem, SIGNAL(triggered(bool)), this, SLOT(exitSystem()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(about()));

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 连接右键菜单信号
    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenuRequested(const QPoint &)));

    closeFileSystem();
}

mainwindow::~mainwindow() {
    delete ui;
}

void mainwindow::onSearchTextChanged(const QString &text) {
    if (!fsModel) {
        return;// 如果没有加载文件系统模型，则什么也不做
    }

    QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
    for (int i = 0; i < fsModel->rowCount(); ++i) {
        filterTreeView(fsModel->index(i, 0), regExp);
    }
}

bool mainwindow::filterTreeView(const QModelIndex &index, const QRegularExpression &regExp) {
    bool match = false;
    if (!index.isValid()) {
        return match;
    }

    QString itemText = fsModel->data(index, Qt::DisplayRole).toString();
    match = regExp.match(itemText).hasMatch();

    for (int i = 0; i < fsModel->rowCount(index); ++i) {
        QModelIndex childIndex = fsModel->index(i, 0, index);
        if (filterTreeView(childIndex, regExp)) {
            match = true;
        }
    }

    ui->treeView->setRowHidden(index.row(), index.parent(), !match);
    return match;
}


// 当前项改变时的槽函数
void mainwindow::onCurrentItemChanged(const QModelIndex &current) {
    if (current.isValid()) {
        // 更新磁盘容量信息
        updateDiskCapacity();
    }
}
// 打开文件系统
void mainwindow::openSystem() {
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
void mainwindow::closeSystem() {
    closeFileSystem();
}

// 格式化
void mainwindow::reformatSystem() {
    reformat rf;
    if (rf.exec() == QDialog::Accepted) {
        delete rf.getFileSystem();
    }
}

// 新建用户
void mainwindow::newUser() {
    if (isOpened()) {
        // TODO fsX->useradd();
    }
}

// 打开目录
void mainwindow::openDirectory() {
}

// 删除目录
void mainwindow::deleteDirectory() {
    if (isOpened()) {
        directory *dir = fsModel->getDirectoryFromIndex(ui->treeView->currentIndex());
        if (!fsX->rm(*dir)) {
            ui->treeView->currentIndex();
            QMessageBox::critical(this, "错误", "删除文件夹失败！");
        }
    }
}

// 目录重命名
void mainwindow::renameDirectory() {
}

// 目录属性
void mainwindow::directoryNature() {
}

// 文件属性
void mainwindow::fileNature() {
}

// 文件重命名
void mainwindow::renameFile() {
}

// 打开文件
void mainwindow::openFile() {
}

// 删除用户
void mainwindow::deleteUser() {
}

// 新开文件
void mainwindow::newFile() {
}

// 新文件夹
void mainwindow::newDirectory() {
}

// 删除文件
void mainwindow::deleteFile() {
}

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
}

// 退出系统
void mainwindow::exitSystem() {
    closeFileSystem();
    QApplication::quit();
}

// TODO 右键菜单处理
void mainwindow::onCustomContextMenuRequested(const QPoint &pos) {
    if (this->fsModel != nullptr) {
        // 获取当前元素
        QModelIndex index = ui->treeView->indexAt(pos);
        if (!index.isValid()) {
            return;
        }

        // 添加菜单
        QMenu contextMenu(this);
        if (this->fsModel->isDirectory(index)) {
            directory *dir = fsModel->getDirectoryFromIndex(index);
            contextMenu.addAction("打开", this, SLOT(openDirectory()));
            contextMenu.addAction("删除", this, SLOT(deleteDirectory()));
            contextMenu.addAction("重命名", this, SLOT(renameDirectory()));
            contextMenu.addAction("新建文件", this, SLOT(newFile()));
            contextMenu.addAction("新建文件夹", this, SLOT(newDirectory()));
            contextMenu.addAction("属性", this, SLOT(directoryNature()));
        } else {
            contextMenu.addAction("打开", this, SLOT(exitSystem()));
            contextMenu.addAction("删除", this, SLOT(deleteFile()));
            contextMenu.addAction("重命名", this, SLOT(renameFile()));
            contextMenu.addAction("属性", this, SLOT(fileNature()));
        }
        contextMenu.exec(ui->treeView->viewport()->mapToGlobal(pos));
    }
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
    updateDiskCapacity();
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
    ui->diskCapacityLabel->setText("");
}

void mainwindow::displayFileSystem() {
    if (fsX != nullptr) {
        // 创建文件系统模型
        if (fsModel == nullptr) {
            fsModel = new FileSystemModel(fsX, fsX->getTree(), this);
        }
        ui->treeView->setModel(fsModel);
    }
}

// 磁盘容量更新函数
void mainwindow::updateDiskCapacity() {
    // 获取磁盘的总容量和剩余容量
    // 更新界面上的标签
    float all = (float) fsX->disk(false);
    float left = (float) fsX->disk(true);
    ui->diskCapacityLabel->setText(QString::fromStdString(getSizeString(left) + "/" + getSizeString(all)));
}

// 根据字节大小获取字符串
string mainwindow::getSizeString(float f) {
    string tmp;
    if (f > 1024.0 * 1024.0 * 1024.0) {
        f /= 1024.0 * 1024.0 * 1024.0;
        tmp = std::to_string(f);
        tmp = tmp.substr(0, tmp.find('.') + 2);
        tmp += "GB";
    } else if (f > 1024.0 * 1024.0) {
        f /= 1024.0 * 1024.0;
        tmp = std::to_string(f);
        tmp = tmp.substr(0, tmp.find('.') + 2);
        tmp += "MB";
    } else if (f > 1024.0) {
        f /= 1024.0;
        tmp = std::to_string(f);
        tmp = tmp.substr(0, tmp.find('.') + 2);
        tmp += "KB";
    } else {
        tmp = std::to_string(f);
        tmp = tmp.substr(0, tmp.find('.') + 2);
        tmp += "B";
    }
    return tmp;
}

bool mainwindow::isOpened() {
    if (fsX == nullptr) {
        QMessageBox::critical(this, "错误", "请先打开一个文件系统，才能执行该操作！");
        return false;
    }
    return true;
}