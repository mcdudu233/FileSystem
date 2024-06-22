//
// Created by 86138 on 2024/6/22.
//

#ifndef FILESYSTEM_FILELISTVIEW_H
#define FILESYSTEM_FILELISTVIEW_H

#include <QListView>
#include <QMenu>
#include <QAction>


QT_BEGIN_NAMESPACE
namespace Ui {
    class FileListView;
}
QT_END_NAMESPACE

class FileListView : public QListView {
    Q_OBJECT

public:
    explicit FileListView(QWidget *parent = nullptr);
    ~FileListView() override;

private:
    Ui::FileListView *ui;
private slots:
    void showContextMenu(const QPoint &pos);
    void onOpenFile();
    void onDeleteFile();
    void onRenameFile();
    void onNewFile();
    void onNewFolder();

private:
    QMenu *contextMenu;
    QAction *openAction;
    QAction *deleteAction;
    QAction *renameAction;
    QAction *newFileAction;
    QAction *newFolderAction;
    QModelIndex currentIndex; // 当前点击的索引
};


#endif//FILESYSTEM_FILELISTVIEW_H
