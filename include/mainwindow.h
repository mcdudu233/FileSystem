//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_MAINWINDOW_H
#define FILESYSTEM_MAINWINDOW_H

#include <QMainWindow>
#include "filelistview.h"



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
    FileListView *fileListView;


public slots:
    void onCurrentItemChanged(const QModelIndex &current, const QModelIndex &previous);// 处理当前项变化
    void onFileSelected(const QModelIndex &index);                                     // 文件选择
    void onFileDoubleClicked(const QModelIndex &index);                                // 双击文件
    void onFileRightClicked(const QModelIndex &index);                                 // 右击文件
    void onSearchTextChanged(const QString &text);                                     // 处理搜索文本
    void showLoginFailedMessage();                                                     // 显示登录信息

signals:
    void showFileManager(); // 显示文件管理系统的信号
    void updateDiskCapacity();
}

;

#endif//FILESYSTEM_MAINWINDOW_H
