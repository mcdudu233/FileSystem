//
// Created by dudu233 on 2024/6/23.
//

// You may need to build the project (run Qt uic code generator) to get "ui_reformat.h" resolved

#include "reformat.h"
#include "../form/ui_reformat.h"

reformat::reformat(QWidget *parent) : QDialog(parent), ui(new Ui::reformat) {
    ui->setupUi(this);

    // 事件连接
    connect(ui->fileSystemComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFileSystemSelected(int)));
    connect(ui->blockSizeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onBlockSizeSelected(int)));
    connect(ui->capacityComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCapacitySelected(int)));
    connect(ui->restoreDefaultsButton, SIGNAL(clicked()), this, SLOT(onRestoreDefaultsClicked()));
    connect(ui->reformatButton, SIGNAL(clicked()), this, SLOT(onReformatButtonClicked()));
    connect(ui->exitButton, SIGNAL(clicked()), this, SLOT(onExitClicked()));

    // 初始采用默认值
    onRestoreDefaultsClicked();

    // 删除已经存在的盘符
    fs::path current_path = fs::current_path();
    for (const auto &entry: fs::directory_iterator(current_path)) {
        if (entry.is_regular_file()) {
            string filename = entry.path().filename().string();
            if (filename.size() >= sizeof(DATA_SUFFIX) - 1 && filename.substr(filename.size() - (sizeof(DATA_SUFFIX) - 1)) == DATA_SUFFIX) {
                filename = filename.substr(0, 1);
                for (int i = 0; i < ui->symbolComboBox->count(); i++) {
                    if (filename == ui->symbolComboBox->itemText(i).toLower().toStdString()) {
                        ui->symbolComboBox->removeItem(i);
                        i--;
                    }
                }
            }
        }
    }
}

reformat::~reformat() {
    delete ui;
}

void reformat::onFileSystemSelected(int index) {
}

void reformat::onBlockSizeSelected(int index) {
}

void reformat::onCapacitySelected(int index) {
}

void reformat::onRestoreDefaultsClicked() {
    // 还原所有选项为默认值
    ui->capacityComboBox->setCurrentIndex(1);
    ui->blockSizeComboBox->setCurrentIndex(1);
    ui->fileSystemComboBox->setCurrentIndex(0);
}

void reformat::onReformatButtonClicked() {
    // 获取块大小
    int block;
    QString tmp = ui->blockSizeComboBox->currentText().replace("B", "");
    if (tmp.contains("K")) {
        block = tmp.replace("K", "").toInt() * 1024;
    } else {
        block = tmp.toInt();
    }
    // 获取容量大小
    int capacity;
    tmp = ui->capacityComboBox->currentText().replace("B", "");
    if (tmp.contains("G")) {
        capacity = tmp.replace("G", "").toInt() * 1024 * 1024 * 1024;
    } else if (tmp.contains("M")) {
        capacity = tmp.replace("M", "").toInt() * 1024 * 1024;
    } else if (tmp.contains("K")) {
        capacity = tmp.replace("K", "").toInt() * 1024;
    } else {
        capacity = tmp.toInt();
    }
    // 获取盘符名
    string symbol = ui->symbolComboBox->currentText().toLower().toStdString();
    // 检测是否有重名
    fs::path current_path = fs::current_path();
    for (const auto &entry: fs::directory_iterator(current_path)) {
        if (entry.is_regular_file()) {
            if (entry.path().filename().string() == symbol + DATA_SUFFIX) {
                QMessageBox::critical(this,
                                      "错误",
                                      "当前已经存在相同类型的盘符！请换一个盘符后重试！");
                return;
            }
        }
    }
    fs = new filesystem(symbol, capacity, block);
    accept();
}


void reformat::onExitClicked() {
    // 关闭当前窗口
    reject();
}

filesystem *reformat::getFileSystem() const {
    return this->fs;
}
