//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_MAINWINDOW_H
#define FILESYSTEM_MAINWINDOW_H

#include "filelistview.h"
#include "filesystem.h"
#include "reformat.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QAction>


QT_BEGIN_NAMESPACE
namespace Ui {
    class mainwindow;
}
QT_END_NAMESPACE

class mainwindow : public QMainWindow {
    Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = nullptr);
    ~mainwindow() override;

private:
    Ui::mainwindow *ui;
    filesystem *fsx = nullptr;
    FileListView *fileListView;

public slots:
    void onCurrentItemChanged(const QModelIndex &current);// 处理当前项变化
    void onFileSelected(const QModelIndex &index);        // 文件选择
    void onFileDoubleClicked(const QModelIndex &index);   // 双击文件
    void onFileRightClicked(const QModelIndex &index);    // 右击文件
    void onSearchTextChanged(const QString &text);        // 处理搜索文本
    void showLoginFailedMessage();                        // 显示登录信息
    void showFileManager();                               // 显示文件管理系统的信号
    void updateDiskCapacity();                            // 更新磁盘容量
    void openButtonCliked();                              // 打开文件按钮按下
    void closeButtonClicked();                            // 关闭文件按钮按下
    void reformatButtonClicked();                         // 格式化按钮按下
    void newUser();                                       // 新建用户
    void deleteUser();                                    // 删除用户
    void newFile();                                       // 新开文件
    void deleteFile();                                    // 删除文件
    void about();                                         //关于
    void newSystem();                                     // 新建系统
    void deleteSystem();                                  // 删除系统
    void exitSystem();                                    // 退出系统

public:
    static vector<fs::path> searchFileSystem();// 搜索当前文件夹下的所有文件系统
    void openFileSystem(QString name);         // 打开文件系统
    void openFileSystem(filesystem *fs);       // 打开文件系统
    void closeFileSystem();                    //关闭文件系统
};

#endif//FILESYSTEM_MAINWINDOW_H
