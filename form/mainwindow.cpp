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
    // 设置窗口图标
    QIcon icon("./res/main.png");
    if (icon.isNull()) {
        icon.addFile("../res/main.png");
    }
    setWindowIcon(icon);
    // 禁用最大化按钮
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    connect(ui->treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onCurrentItemChanged(const QModelIndex &)));
    connect(ui->treeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onTreeViewDoubleClicked(const QModelIndex &)));

    connect(ui->searchEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onSearchTextChanged(const QString &)));
    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(openSystem()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(closeSystem()));

    connect(ui->actionNewFileSystem, SIGNAL(triggered(bool)), this, SLOT(reformatSystem()));
    connect(ui->actionDeleteFileSystem, SIGNAL(triggered(bool)), this, SLOT(deleteSystem()));
    connect(ui->actionExitSystem, SIGNAL(triggered(bool)), this, SLOT(exitSystem()));

    connect(ui->actionNewUser, SIGNAL(triggered(bool)), this, SLOT(newUser()));
    connect(ui->actionDeleteUser, SIGNAL(triggered(bool)), this, SLOT(deleteUser()));
    connect(ui->actionLoginUser, SIGNAL(triggered(bool)), this, SLOT(loginUser()));
    connect(ui->actionChangeUser, SIGNAL(triggered(bool)), this, SLOT(changeUser()));
    connect(ui->actionPasswordUser, SIGNAL(triggered(bool)), this, SLOT(passwordUser()));

    connect(ui->reformatButton, SIGNAL(clicked()), this, SLOT(reformatSystem()));
    connect(ui->actionNewsystem, SIGNAL(triggered(bool)), this, SLOT(reformatSystem()));
    connect(ui->actionOpenSystem, SIGNAL(triggered(bool)), this, SLOT(openSystem()));
    connect(ui->actionCloseSystem, SIGNAL(triggered(bool)), this, SLOT(closeSystem()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(about()));

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 连接右键菜单信号
    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenuRequested(const QPoint &)));

    closeFileSystem();
}

mainwindow::~mainwindow() {
    delete ui;
}


void mainwindow::onSearchTextChanged(const QString &text) {
    if (text.contains("请输入文件名") || text == "请输入文件") {
        ui->searchEdit->selectAll();
        return;
    }
    if (!fsModel) return;
    if (fsModel != ui->treeView->model()) return;

    fsModel->clearSearchResults();

    QRegularExpression regExp(text, QRegularExpression::CaseInsensitiveOption);
    bool hasMatch = false;

    // 收起所有目录
    //    for (int i = 0; i < fsModel->rowCount(); ++i) {
    //        ui->treeView->collapse(fsModel->index(i, 0));
    //    }

    // 遍历模型的根项
    for (int i = 0; i < fsModel->rowCount(); ++i) {
        QModelIndex rootIndex = fsModel->index(i, 0);
        if (filterAndExpandTreeView(rootIndex, regExp)) {
            ui->treeView->setModel(fsModel);
            ui->treeView->selectionModel()->clearSelection();
            ui->treeView->selectionModel()->setCurrentIndex(fsModel->index(i, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            hasMatch = true;
        }
    }

    if (!hasMatch) {
        QMessageBox::critical(this, "未找到", "请输入正确的文件名！");
    }
}

bool mainwindow::filterAndExpandTreeView(const QModelIndex &index, const QRegularExpression &regExp) {
    if (!index.isValid()) {
        return false;
    }

    bool match;
    QString itemText = fsModel->data(index, Qt::DisplayRole).toString();
    match = regExp.match(itemText).hasMatch();

    // 只展开包含匹配文件的目录
    if (match) {
        ui->treeView->expandAll();
        // expandParents(index);
        // ui->treeView->setRowHidden(index.row(), index.parent(), false);
    } else {
        // ui->treeView->setRowHidden(index.row(), index.parent(), true);
    }

    for (int i = 0; i < fsModel->rowCount(index); ++i) {
        QModelIndex childIndex = fsModel->index(i, 0, index);
        if (filterAndExpandTreeView(childIndex, regExp)) {
            match = true;
        }
    }

    return match;
}

void mainwindow::expandParents(const QModelIndex &index) {
    QModelIndex parentIndex = index.parent();
    if (parentIndex.isValid()) {
        expandParents(parentIndex);
        ui->treeView->expand(parentIndex);
    }
}


// 当前项改变时的槽函数
void mainwindow::onCurrentItemChanged(const QModelIndex &current) {
    if (current.isValid()) {
        // 更新磁盘容量信息或其他操作
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

void mainwindow::deleteSystem() {
    if (fsX != nullptr) {
        closeFileSystem();
    }

    const vector<fs::path> &fileSystem = searchFileSystem();
    if (fileSystem.empty()) {
        QMessageBox::critical(this, "错误", "您还没有创建系统，怎么能删除呢？");
        return;
    }
    QStringList ls;
    for (auto f: fileSystem) {
        ls.push_back(QString::fromStdString(f.filename().string()).split(".")[0].toUpper());
    }
    bool ok;
    int index = ls.indexOf(QInputDialog::getItem(this, "删除文件系统", "请选择要删除的盘符：", ls, 0, true, &ok));
    if (ok) {
        if (std::remove(fileSystem[index].string().c_str())) {
            QMessageBox::critical(this, "错误", "无法删除该文件系统！");
        }
    }
}

// 新建用户
void mainwindow::newUser() {
    if (isOpened()) {
        bool ok1, ok2;
        QString name = QInputDialog::getText(this, "新建用户", "用户名：", QLineEdit::Normal, "用户名", &ok1);
        if (ok1) {
            QString password = QInputDialog::getText(this, "新建用户", "密码：", QLineEdit::Normal, "", &ok2);
            if (ok2) {
                if (name.isEmpty()) {
                    QMessageBox::critical(this, "错误", "用户名不允许为空！");
                } else {
                    if (fsX->useradd(name.toStdString(), password.toStdString(), false) == -1) {
                        QMessageBox::critical(this, "错误", "添加用户失败！换一个名字吧！");
                    }
                }
            }
        }
    }
}

// 删除用户
void mainwindow::deleteUser() {
    if (isOpened()) {
        QStringList ls;
        for (auto u: fsX->usrs()) {
            ls.push_back("ID：" + QString::fromStdString(std::to_string(u.getUid())) + "\t用户名：" + QString::fromStdString(u.getName()));
        }

        bool ok;
        int index = ls.indexOf(QInputDialog::getItem(this, "删除用户", "请选择要删除的用户：", ls, 0, true, &ok));
        if (ok) {
            if (!fsX->userdel(index)) {
                QMessageBox::critical(this, "错误", "删除用户失败！未知错误！");
            }
        }
    }
}

void mainwindow::loginUser() {
    if (isOpened()) {
        login l(fsX, this);
        l.exec();
    }
}

void mainwindow::changeUser() {
    loginUser();
}

void mainwindow::passwordUser() {
    if (fsX == nullptr) {
        QMessageBox::warning(this, "错误", "请先打开文件系统");
        return;
    }
    bool ok;
    QStringList userNames;
    for (user usr: fsX->getUsers()) {
        userNames << QString::fromStdString(usr.getName());
    }
    QString selectedUser = QInputDialog::getItem(this, "修改用户密码", "请选择要修改密码的用户：", userNames, 0, false, &ok);
    if (!ok || selectedUser.isEmpty()) {
        return;
    }
    user *usr = fsX->getUserByName(selectedUser.toStdString());
    if (!usr) {
        QMessageBox::warning(this, "错误", "未找到该用户");
        return;
    }
    QString oldPassword = QInputDialog::getText(this, "修改用户密码", "请输入旧密码：", QLineEdit::Password, "", &ok);
    if (!ok) {
        return;
    }
    if (!usr->checkPassword(oldPassword.toStdString())) {
        QMessageBox::warning(this, "错误", "旧密码错误");
        return;
    }
    QString newPassword = QInputDialog::getText(this, "修改用户密码", "请输入新密码：", QLineEdit::Password, "", &ok);
    if (!ok) {
        return;
    }
    QString confirmPassword = QInputDialog::getText(this, "修改用户密码", "请确认新密码：", QLineEdit::Password, "", &ok);
    if (!ok) {
        return;
    }
    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, "错误", "两次输入的密码不一致");
        return;
    }
    usr->setPassword(newPassword.toStdString());
    QMessageBox::information(this, "成功", "密码修改成功");
}

void FileSystemModel::clearSearchResults() {
    searchResults.clear();
}

void FileSystemModel::addSearchResult(const QModelIndex &index) {
    searchResults.append(QPersistentModelIndex(index));
}

// 打开目录
void mainwindow::openDirectory() {
    if (isOpened()) {
        QModelIndex index = ui->treeView->currentIndex();
        if (index.isValid() && ui->treeView->model()->hasChildren(index)) {
            ui->treeView->expand(index);
        } else {
            QMessageBox::critical(this, "错误", "该目录下没有文件！");
        }
    }
}

// 删除目录
void mainwindow::deleteDirectory() {
    if (isOpened()) {
        directory *dir = fsModel->getDirectoryFromIndex(ui->treeView->currentIndex());
        if (fsX->rm(*dir)) {
            displayFileSystem();
        } else {
            QMessageBox::critical(this, "错误", "删除文件夹失败！");
        }
    }
}

// 目录重命名
void mainwindow::renameDirectory() {
    if (isOpened()) {
        QModelIndex index = ui->treeView->currentIndex();
        if (fsModel->isDirectory(index)) {
            bool ok;
            QString newName = QInputDialog::getText(this, "重命名文件夹", "新的文件夹名称：", QLineEdit::Normal, fsModel->getDirectoryFromIndex(index)->getName().c_str(), &ok);
            if (ok && !newName.isEmpty()) {
                QModelIndex parentIndex = index.parent();
                // 检查同一目录下是否有同名的文件夹
                for (int i = 0; i < fsModel->rowCount(parentIndex); ++i) {
                    QModelIndex siblingIndex = fsModel->index(i, 0, parentIndex);
                    if (fsModel->data(siblingIndex, Qt::DisplayRole).toString() == newName && siblingIndex != index) {
                        QMessageBox::critical(this, "错误", "同一目录下已存在同名文件夹！");
                        return;
                    }
                }
                directory *dir = fsModel->getDirectoryFromIndex(index);
                dir->setName(newName.toStdString());// 直接设置新名称
                displayFileSystem();                // 更新显示
            } else {
                QMessageBox::critical(this, "错误", "文件夹名不允许为空！");
            }
        } else {
            QMessageBox::critical(this, "错误", "请选择一个目录！");
        }
    }
}

// 目录属性
void mainwindow::directoryNature() {
    if (isOpened()) {
        directory *d = fsModel->getDirectoryFromIndex(ui->treeView->currentIndex());
        // 创建一个对话框
        QDialog propertiesDialog(this);
        propertiesDialog.setWindowTitle("文件夹属性");
        // 使用QFormLayout布局来对齐标签和值
        QFormLayout formLayout;
        // 添加属性到布局
        formLayout.addRow(new QLabel("文件夹名:"), new QLabel(d->getName().c_str()));
        formLayout.addRow(new QLabel("所属用户:"), new QLabel(fsX->userbyid(d->getUser()).getName().c_str()));
        formLayout.addRow(new QLabel("包含文件数量:"), new QLabel(QString::number(d->getFiles()->size() + d->getDirectories()->size())));
        formLayout.addRow(new QLabel("创建日期:"), new QLabel(getTimeString(d->getCreateTime()).c_str()));
        formLayout.addRow(new QLabel("修改日期:"), new QLabel(getTimeString(d->getModifyTime()).c_str()));
        // 将布局设置到对话框
        propertiesDialog.setLayout(&formLayout);
        // 显示对话框
        propertiesDialog.exec();
    }
}

// 文件属性
void mainwindow::fileNature() {
    if (isOpened()) {
        file *f = fsModel->getFileFromIndex(ui->treeView->currentIndex());
        // 创建一个对话框
        QDialog propertiesDialog(this);
        propertiesDialog.setWindowTitle("文件属性");
        // 使用QFormLayout布局来对齐标签和值
        QFormLayout formLayout;
        // 添加属性到布局
        formLayout.addRow(new QLabel("文件名:"), new QLabel(f->getName().c_str()));
        formLayout.addRow(new QLabel("所属用户:"), new QLabel(fsX->userbyid(f->getUser()).getName().c_str()));
        formLayout.addRow(new QLabel("文件大小:"), new QLabel(getSizeString(f->getSize()).c_str()));
        formLayout.addRow(new QLabel("创建日期:"), new QLabel(getTimeString(f->getCreateTime()).c_str()));
        formLayout.addRow(new QLabel("修改日期:"), new QLabel(getTimeString(f->getModifyTime()).c_str()));
        // 将布局设置到对话框
        propertiesDialog.setLayout(&formLayout);
        // 显示对话框
        propertiesDialog.exec();
    }
}

// 文件重命名
void mainwindow::renameFile() {
    if (isOpened()) {
        QModelIndex index = ui->treeView->currentIndex();
        if (!fsModel->isDirectory(index)) {
            bool ok;
            QString newName = QInputDialog::getText(this, "重命名文件", "新的文件名称：", QLineEdit::Normal, fsModel->getFileFromIndex(index)->getName().c_str(), &ok);
            if (ok && !newName.isEmpty()) {
                QModelIndex parentIndex = index.parent();
                // 检查同一目录下是否有同名的文件
                for (int i = 0; i < fsModel->rowCount(parentIndex); ++i) {
                    QModelIndex siblingIndex = fsModel->index(i, 0, parentIndex);
                    if (fsModel->data(siblingIndex, Qt::DisplayRole).toString() == newName && siblingIndex != index) {
                        QMessageBox::critical(this, "错误", "同一目录下已存在同名文件！");
                        return;
                    }
                }
                file *file = fsModel->getFileFromIndex(index);
                file->setName(newName.toStdString());// 直接设置新名称
                displayFileSystem();                 // 更新显示
            } else {
                QMessageBox::critical(this, "错误", "文件名不允许为空！");
            }
        } else {
            QMessageBox::critical(this, "错误", "请选择一个文件！");
        }
    }
}

// 打开文件
void mainwindow::openFile() {
    if (isOpened()) {
        file *f = fsModel->getFileFromIndex(ui->treeView->currentIndex());
        QList<QString> sp = QString::fromStdString(f->getName()).split(".");
        if (sp.size() == 1) {
            // 文件没有后缀
            QMessageBox::critical(this, "警告", "文件没有后缀！无法解析！");
        } else {
            // 得到后缀
            QString suffix = sp[sp.size() - 1];
            if (suffix == "txt") {
                textedit te(f, this);
                if (te.exec() == QDialog::Accepted) {
                    updateDiskCapacity();
                }
            } else {
                QMessageBox::information(this, "提示", "无法使用内置的txt查看器打开！请选择系统程序打开！");
                // 保存文件到本地
                QString tempFilePath = QDir::tempPath() + "/" + f->getName().c_str();
                QFile file(tempFilePath);
                if (file.open(QIODevice::WriteOnly)) {
                    const char *data = f->readFile();
                    file.write(data);
                    file.close();
                }
                // 使用系统默认程序打开文件
                QDesktopServices::openUrl(QUrl::fromLocalFile(tempFilePath));
                // 添加文件到监视器 如果文件内容改变 写回本系统
                QThread *monitorThread = QThread::create([=]() {
                    QDateTime lastModified = (new QFileInfo(tempFilePath))->lastModified();
                    while (true) {
                        QThread::sleep(1);// 每秒检查一次
                        QDateTime newLastModified = QFileInfo(tempFilePath).lastModified();
                        if (newLastModified != lastModified) {
                            lastModified = newLastModified;

                            // 文件发生变化，读取新内容并更新系统中的数据
                            QFile file(tempFilePath);
                            if (file.open(QIODevice::ReadOnly)) {
                                QByteArray newData = file.readAll();
                                file.close();
                                // 将新数据写回系统中的文件对象
                                f->writeFile(newData.data(), newData.size());
                                // 更新磁盘容量显示
                                updateDiskCapacity();
                            }
                        }
                    }
                });
                // 启动监控线程
                monitorThread->start();
            }
        }
    }
}

// 新开文件
void mainwindow::newFile() {
    if (isOpened()) {
        bool ok;
        QString text = QInputDialog::getText(this, "新建文件", "文件名：", QLineEdit::Normal, "新文件.txt", &ok);
        if (ok) {
            if (text.isEmpty()) {
                QMessageBox::critical(this, "错误", "文件名不允许为空！");
            } else {
                directory *dir;
                if (ui->treeView->currentIndex() == ui->treeView->rootIndex()) {
                    dir = fsX->getTree();
                } else {
                    dir = fsModel->getDirectoryFromIndex(ui->treeView->currentIndex());
                }
                if (fsX->touch(*dir, text.toStdString())) {
                    displayFileSystem();
                } else {
                    QMessageBox::critical(this, "错误", "当前文件已经存在！换一个名字吧！");
                }
            }
        }
    }
}

// 新文件夹
void mainwindow::newDirectory() {
    if (isOpened()) {
        bool ok;
        QString text = QInputDialog::getText(this, "新建文件夹", "文件夹名称：", QLineEdit::Normal, "新目录", &ok);
        if (ok) {
            if (text.isEmpty()) {
                QMessageBox::critical(this, "错误", "文件夹名不允许为空！");
            } else {
                directory *dir;
                if (ui->treeView->currentIndex() == ui->treeView->rootIndex()) {
                    dir = fsX->getTree();
                } else {
                    dir = fsModel->getDirectoryFromIndex(ui->treeView->currentIndex());
                }
                if (fsX->mkdir(*dir, text.toStdString())) {
                    displayFileSystem();
                } else {
                    QMessageBox::critical(this, "错误", "当前文件夹已经存在！换一个名字吧！");
                }
            }
        }
    }
}

// 删除文件
void mainwindow::deleteFile() {
    if (isOpened()) {
        file *file = fsModel->getFileFromIndex(ui->treeView->currentIndex());
        if (fsX->rm(*file)) {
            displayFileSystem();
        } else {
            QMessageBox::critical(this, "错误", "删除文件失败！");
        }
    }
}

//关于
void mainwindow::about() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("关于");
    msgBox.setText("该程序遵循 GPL-3.0 协议！");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

// 退出系统
void mainwindow::exitSystem() {
    closeFileSystem();
    QApplication::quit();
}

// 右键菜单处理
void mainwindow::onCustomContextMenuRequested(const QPoint &pos) {
    if (this->fsModel != nullptr) {
        // 获取当前元素
        QModelIndex index = ui->treeView->indexAt(pos);

        // 添加菜单
        QMenu contextMenu(this);
        if (index.isValid()) {
            if (this->fsModel->isDirectory(index)) {
                contextMenu.addAction("打开", this, SLOT(openDirectory()));
                contextMenu.addAction("删除", this, SLOT(deleteDirectory()));
                contextMenu.addAction("重命名", this, SLOT(renameDirectory()));
                contextMenu.addAction("新建文件", this, SLOT(newFile()));
                contextMenu.addAction("新建文件夹", this, SLOT(newDirectory()));
                contextMenu.addAction("属性", this, SLOT(directoryNature()));
            } else {
                contextMenu.addAction("打开", this, SLOT(openFile()));
                contextMenu.addAction("删除", this, SLOT(deleteFile()));
                contextMenu.addAction("重命名", this, SLOT(renameFile()));
                contextMenu.addAction("属性", this, SLOT(fileNature()));
            }
        } else {
            // 当右键空白区域的时候
            contextMenu.addAction("新建文件", this, SLOT(newFile()));
            contextMenu.addAction("新建文件夹", this, SLOT(newDirectory()));
            ui->treeView->setCurrentIndex(ui->treeView->rootIndex());
        }
        contextMenu.exec(ui->treeView->viewport()->mapToGlobal(pos));
    }
}

// 双击事件
void mainwindow::onTreeViewDoubleClicked(const QModelIndex &index) {
    if (!index.isValid()) {
        return;
    }

    if (fsModel->isDirectory(index)) {
        // 如果双击的是目录，展开该目录
        openDirectory();
    } else {
        // 如果双击的是文件，打开该文件
        openFile();
    }
}

// 重写关闭事件
void mainwindow::closeEvent(QCloseEvent *event) {
    if (fsX != nullptr) {
        // 弹出确认对话框
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "警告", "您确定要退出系统吗？",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // 如果确认关闭，则接受关闭事件
            closeFileSystem();
            event->accept();
        } else {
            //忽略关闭信号，阻止窗体关闭
            event->ignore();
        }
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
    if (fsModel == nullptr) {
        fsModel = new FileSystemModel(fsX, fsX->getTree(), this);
    }
    ui->treeView->setModel(fsModel);
    ui->openButton->setDisabled(true);
    ui->closeButton->setDisabled(false);
    ui->actionOpenSystem->setDisabled(true);
    ui->actionCloseSystem->setDisabled(false);
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
    ui->actionOpenSystem->setDisabled(false);
    ui->actionCloseSystem->setDisabled(true);
    ui->diskCapacityLabel->setText("");
}

void mainwindow::displayFileSystem() {
    if (fsX != nullptr) {
        // 创建文件系统模型
        if (fsModel == nullptr) {
            fsModel = new FileSystemModel(fsX, fsX->getTree(), this);
        } else {
            delete fsModel;
            fsModel = new FileSystemModel(fsX, fsX->getTree(), this);
            ui->treeView->setModel(fsModel);
        }
        updateDiskCapacity();
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

// 根据时间获得字符串
string mainwindow::getTimeString(const std::chrono::system_clock::time_point &tp) {
    // 获取 time_point 的时间点
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    // 将 time 转换为 tm 结构
    std::tm tm = *std::localtime(&time);
    // 使用 stringstream 格式化时间字符串
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

bool mainwindow::isOpened() {
    if (fsX == nullptr) {
        QMessageBox::critical(this, "错误", "请先打开一个文件系统，才能执行该操作！");
        return false;
    }
    return true;
}
