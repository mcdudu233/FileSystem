//
// Created by dudu233 on 2024/6/30.
//

#ifndef FILESYSTEM_TEXTEDIT_H
#define FILESYSTEM_TEXTEDIT_H

#include "file.h"
#include <QCloseEvent>
#include <QDialog>
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui {
    class textedit;
}
QT_END_NAMESPACE

class textedit : public QDialog {
    Q_OBJECT

public:
    explicit textedit(file *f, QWidget *parent = nullptr);
    ~textedit() override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::textedit *ui;
    file *fi;

public slots:
    void save();// 保存文件
};


#endif//FILESYSTEM_TEXTEDIT_H
