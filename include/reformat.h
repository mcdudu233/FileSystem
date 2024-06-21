//
// Created by 86138 on 2024/6/21.
//

#ifndef FILESYSTEM_REFORMAT_H
#define FILESYSTEM_REFORMAT_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui {
    class reformat;
}
QT_END_NAMESPACE

class reformat : public QWidget {
    Q_OBJECT

public:
    explicit reformat(QWidget *parent = nullptr);
    ~reformat() override;

private:
    Ui::reformat *ui;
};


#endif//FILESYSTEM_REFORMAT_H
