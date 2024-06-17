//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_MAINWINDOW_H
#define FILESYSTEM_MAINWINDOW_H

#include <QMainWindow>


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
};


#endif//FILESYSTEM_MAINWINDOW_H
