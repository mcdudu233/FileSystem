//
// Created by dudu233 on 2024/6/23.
//

#ifndef FILESYSTEM_REFORMAT_H
#define FILESYSTEM_REFORMAT_H

#include "filesystem.h"
#include <QDialog>
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui {
    class reformat;
}
QT_END_NAMESPACE

class reformat : public QDialog {
    Q_OBJECT

public:
    explicit reformat(QWidget *parent = nullptr);
    ~reformat() override;
    filesystem *getFileSystem() const;

private:
    Ui::reformat *ui;
    filesystem *fs;

public slots:
    void onReformatButtonClicked();      // 开始格式化
    void onExitClicked();                // 关闭格式化窗口
    void onBlockSizeSelected(int index); // 分块大小选择
    void onRestoreDefaultsClicked();     // 还原默认值
    void onCapacitySelected(int index);  // 磁盘容量选择
    void onFileSystemSelected(int index);// 文件系统选择
};


#endif//FILESYSTEM_REFORMAT_H
