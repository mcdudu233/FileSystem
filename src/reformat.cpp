//
// Created by 86138 on 2024/6/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_reformat.h" resolved

#include "reformat.h"
#include "../form/ui_reformat.h"

// 请修改QT界面中相应的模块名称
reformat::reformat(QWidget *parent) : QWidget(parent), ui(new Ui::reformat) {
    ui->setupUi(this);
    connect(ui->fileSystemComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFileSystemSelected(int)));
    connect(ui->blockSizeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onBlockSizeSelected(int)));
    connect(ui->capacityComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCapacitySelected(int)));
    connect(ui->restoreDefaultsButton, SIGNAL(clicked()), this, SLOT(onRestoreDefaultsClicked()));
    connect(ui->reformatButton, SIGNAL(clicked()), this, SLOT(onReformatButtonClicked()));
    connect(ui->exitButton, SIGNAL(clicked()), this, SLOT(onExitClicked()));
}

reformat::~reformat() {
    delete ui;
}
// ... 现有的槽函数实现


void reformat::onFileSystemSelected(int index) {
}

void reformat::onBlockSizeSelected(int index) {
}

void reformat::onCapacitySelected(int index) {
    // 用户输入了新的容量，可以在这里处理
    //    qDebug() << "Selected capacity:" << capacity;
}

void reformat::onRestoreDefaultsClicked() {
    // 还原默认值
    // ... 现有的还原默认值逻辑

    // 容量默认为空
}

void reformat::onReformatButtonClicked() {
}


void reformat::onExitClicked() {
    // 处理退出前的清理工作，如果需要的话
    QApplication::quit();// 关闭应用程序
}