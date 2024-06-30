//
// Created by dudu233 on 2024/6/30.
//

// You may need to build the project (run Qt uic code generator) to get "ui_textedit.h" resolved

#include "textedit.h"
#include "ui_textedit.h"


textedit::textedit(file *f, QWidget *parent) : QDialog(parent), ui(new Ui::textedit) {
    ui->setupUi(this);
    ui->filename->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    setWindowTitle("文本编辑器");
    connect(ui->save, SIGNAL(clicked(bool)), this, SLOT(save()));
    // 设置窗口图标
    QIcon icon("./res/main.png");
    if (icon.isNull()) {
        icon.addFile("../res/main.png");
    }
    setWindowIcon(icon);

    // 读取文件内容并显示
    this->fi = f;
    ui->filename->setText(f->getName().c_str());
    ui->text->setText(f->readFile());
}

textedit::~textedit() {
    delete ui;
}

void textedit::save() {
    string text = ui->text->toPlainText().toStdString();
    char *data = new char[text.length() + 1];
    strcpy(data, text.c_str());
    this->fi->writeFile(data, text.length());
    accept();
}

void textedit::closeEvent(QCloseEvent *event) {
    // 弹出确认对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "警告", "是否保存文件？",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        save();
    } else {
        // 取消保存文件 直接关闭
        event->accept();
    }
}
