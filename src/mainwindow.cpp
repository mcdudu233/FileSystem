//
// Created by dudu233 on 2024/6/17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include "mainwindow.h"
#include "../form/ui_mainwindow.h"

// 修改相应的模块名
mainwindow::mainwindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainwindow) {
    ui->setupUi(this);
    connect(searchEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    connect(treeView, &QTreeView::clicked, this, &MainWindow::onCurrentItemChanged);
    connect(listView, &QListView::clicked, this, &MainWindow::onFileSelected);
    connect(listView, &QListView::doubleClicked, this, &MainWindow::onFileDoubleClicked);
    connect(listView, &QListView::customContextMenuRequested, this, &MainWindow::onFileRightClicked);
    connect(searchEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);


}

mainwindow::~mainwindow() {
    delete ui;
}
void mainwindow::onFileSelected(const QModelIndex &index) {
    // 文件被选中，可以在这里处理
}

void  mainwindow::onFileDoubleClicked(const QModelIndex &index) {
    // 文件被双击，可以在这里处理
}

void  mainwindow::onFileRightClicked(const QModelIndex &index) {
    // 文件被右键点击，可以在这里处理
}

void  mainwindow::onSearchTextChanged(const QString &text) {
    // 搜索文本发生变化，可以在这里处理搜索逻辑
}