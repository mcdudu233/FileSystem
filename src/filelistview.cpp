//
// Created by 86138 on 2024/6/22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FileListView.h" resolved

#include "filelistview.h"
#include "../form/ui_FileListView.h"
#include <QMessageBox>
#include <QInputDialog>


FileListView::FileListView(QWidget *parent) : QListView(parent), ui(new Ui::FileListView) {
    ui->setupUi(this);
    // 创建上下文菜单
    contextMenu = new QMenu(this);
    openAction = new QAction("Open", this);
    deleteAction = new QAction("Delete", this);
    renameAction = new QAction("Rename", this);
    newFileAction = new QAction("New File", this);
    newFolderAction = new QAction("New Folder", this);

    contextMenu->addAction(openAction);
    contextMenu->addAction(deleteAction);
    contextMenu->addAction(renameAction);
    contextMenu->addSeparator();
    contextMenu->addAction(newFileAction);
    contextMenu->addAction(newFolderAction);

    // 连接信号槽
    connect(openAction, &QAction::triggered, this, &FileListView::onOpenFile);
    connect(deleteAction, &QAction::triggered, this, &FileListView::onDeleteFile);
    connect(renameAction, &QAction::triggered, this, &FileListView::onRenameFile);
    connect(newFileAction, &QAction::triggered, this, &FileListView::onNewFile);
    connect(newFolderAction, &QAction::triggered, this, &FileListView::onNewFolder);

    // 设置视图的上下文菜单策略
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QListView::customContextMenuRequested, this, &FileListView::showContextMenu);
}
FileListView::~FileListView() {
    delete ui;
}
void FileListView::showContextMenu(const QPoint &pos)
{
    QModelIndex index = indexAt(pos);
    if (index.isValid()) {
        contextMenu->exec(viewport()->mapToGlobal(pos));
    }
}

void FileListView::onOpenFile()
{
    // 打开文件的逻辑
    QMessageBox::information(this, "Open", "Open file action triggered");
}

void FileListView::onDeleteFile()
{
    // 删除文件的逻辑
    QMessageBox::information(this, "Delete", "Delete file action triggered");
}

void FileListView::onRenameFile()
{
    // 重命名文件的逻辑
    QMessageBox::information(this, "Rename", "Rename file action triggered");
}

void FileListView::onNewFile()
{
    bool ok;
    QString fileName = QInputDialog::getText(this, "New File", "Enter file name:", QLineEdit::Normal, "", &ok);
    if (ok && !fileName.isEmpty()) {
        // 创建新文件的逻辑
        QMessageBox::information(this, "New File", QString("New file '%1' created").arg(fileName));
    }
}

void FileListView::onNewFolder()
{
    bool ok;
    QString folderName = QInputDialog::getText(this, "New Folder", "Enter folder name:", QLineEdit::Normal, "", &ok);
    if (ok && !folderName.isEmpty()) {
        // 创建新文件夹的逻辑
        QMessageBox::information(this, "New Folder", QString("New folder '%1' created").arg(folderName));
    }
}