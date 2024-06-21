//
// Created by 86138 on 2024/6/21.
//

// You may need to build the project (run Qt uic code generator) to get "ui_reformat.h" resolved

#include "reformat.h"
#include "../form/ui_reformat.h"


reformat::reformat(QWidget *parent) : QWidget(parent), ui(new Ui::reformat) {
    ui->setupUi(this);
}

reformat::~reformat() {
    delete ui;
}
