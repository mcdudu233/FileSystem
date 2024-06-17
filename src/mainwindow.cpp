//
// Created by dudu233 on 2024/6/17.
//

// You may need to build the project (run Qt uic code generator) to get "ui_mainwindow.h" resolved

#include "mainwindow.h"
#include "../form/ui_mainwindow.h"


mainwindow::mainwindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainwindow) {
    ui->setupUi(this);
}

mainwindow::~mainwindow() {
    delete ui;
}
