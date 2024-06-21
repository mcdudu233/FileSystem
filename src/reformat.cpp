//
// Created by 86138 on 2024/6/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_reformat.h" resolved

#include "reformat.h"
#include "../form/ui_reformat.h"

// 请修改QT界面中相应的模块名称
reformat::reformat(QWidget *parent) : QWidget(parent), ui(new Ui::reformat) {
    ui->setupUi(this);
    connect(fileSystemComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFileSystemSelected(int)));
    connect(blockSizeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onBlockSizeSelected(int)));
    connect(capacityComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCapacitySelected(int)));
    connect(restoreDefaultsButton, SIGNAL(clicked()), this, SLOT(onRestoreDefaultsClicked()));
    connect(reformatButton, SIGNAL(clicked()), this, SLOT(onReformatButtonClicked()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(onExitClicked()));



}

reformat::~reformat() {
    delete ui;
}
