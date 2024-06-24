//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_MAINWINDOW_H
#define FILESYSTEM_MAINWINDOW_H

#include "filelistview.h"
#include "filesystem.h"
#include "reformat.h"
#include <QAction>
#include <QComboBox>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <utility>


QT_BEGIN_NAMESPACE
namespace Ui {
    class mainwindow;
}
QT_END_NAMESPACE

// 自定义的文件系统模型类
class FileSystemModel : public QAbstractItemModel {
    Q_OBJECT

private:
    filesystem *fsX = nullptr;
    directory *root = nullptr;
    enum class ItemType { Directory,
                          File };
    typedef struct {
        void *ptr;
        ItemType type;
    } ItemInfo;

public:
    FileSystemModel(filesystem *fsX, directory *root, QObject *parent = nullptr)
        : fsX(fsX), root(root), QAbstractItemModel(parent) {
    }

    ~FileSystemModel() {
    }

public:
    /* 必须实现的虚函数 */

    // 列数量
    int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        return ListSize;
    };

    // 行数量
    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        if (parent.column() > 0) {
            return 0;
        }

        directory *parentDir;
        if (parent.isValid()) {
            if (isDirectory(parent)) {
                parentDir = getDirectoryFromIndex(parent);
            } else {
                return 0;
            }
        } else {
            parentDir = root;
        }
        return parentDir->getDirectories()->size() + parentDir->getFiles()->size();
    };

    Qt::ItemFlags flags(const QModelIndex &index) const override {
        if (!index.isValid()) {
            return Qt::NoItemFlags;
        }
        return QAbstractItemModel::flags(index);
    };

    // 获取每一行的表项数据
    QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid() || role != Qt::DisplayRole) {
            return {};
        }

        if (isDirectory(index)) {
            directory *dir = getDirectoryFromIndex(index);
            if (!dir) {
                return {};
            }
            switch (index.column()) {
                case 0:
                    return QString::fromStdString(dir->getName());
                case 1:
                    return dir->getDirectories()->size() + dir->getFiles()->size();// 子目录和文件总数
                case 2:
                    return "目录";
                case 3:
                    return QString::fromStdString(timePointToString(dir->getModifyTime()));
                default:
                    return {};
            }
        } else {
            file *f = getFileFromIndex(index);
            if (!f) {
                return {};
            }
            switch (index.column()) {
                case 0:
                    return QString::fromStdString(f->getName());
                case 1:
                    return f->getSize();
                case 2:
                    return "文件";
                case 3:
                    return QString::fromStdString(timePointToString(f->getModifyTime()));
                default:
                    return {};
            }
        }
    };

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override {
        if (!hasIndex(row, column, parent)) {
            return {};
        }

        directory *parentDir;
        if (!parent.isValid()) {
            parentDir = root;// 根目录
        } else {
            parentDir = getDirectoryFromIndex(parent);
        }
        if (row < parentDir->getDirectories()->size()) {
            directory *childDir = &parentDir->getDirectories()->at(row);
            auto *info = new ItemInfo{static_cast<void *>(childDir), ItemType::Directory};
            return createIndex(row, column, info);
        } else {
            file *childFile = &parentDir->getFiles()->at(row - parentDir->getDirectories()->size());
            auto *info = new ItemInfo{static_cast<void *>(childFile), ItemType::File};
            return createIndex(row, column, info);
        }
    };

    QModelIndex parent(const QModelIndex &index) const override {
        if (!index.isValid()) {
            return {};
        }

        if (isDirectory(index)) {
            directory *childDir = getDirectoryFromIndex(index);
            if (!childDir || childDir->getFather().empty()) {
                return {};
            }

            directory *parentDir = fsX->getFatherByName(*childDir);
            if (parentDir) {
                auto *parentInfo = new ItemInfo{static_cast<void *>(parentDir), ItemType::Directory};
                return createIndex(0, 0, parentInfo);// 注意：这里需要根据实际情况计算父目录的行号
            }
        } else {
            file *childFile = getFileFromIndex(index);
            if (!childFile) {
                return {};
            }

            directory *parentDir = fsX->getFatherByName(*childFile);
            if (parentDir) {
                auto *parentInfo = new ItemInfo{static_cast<void *>(parentDir), ItemType::Directory};
                return createIndex(0, 0, parentInfo);// 注意：这里需要根据实际情况计算父目录的行号
            }
        }
        return {};
    };

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role != Qt::DisplayRole) {
            return {};
        }

        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return QString("文件名");
                case 1:
                    return QString("大小");
                case 2:
                    return QString("类型");
                case 3:
                    return QString("修改时间");
                default:
                    return {};
            }
        }
        return {};
    };

public:
    /* 工具 */
    bool isDirectory(const QModelIndex &index) const {
        if (!index.isValid()) {
            return false;
        }
        auto *info = static_cast<ItemInfo *>(index.internalPointer());
        return info->type == ItemType::Directory;
    }

    directory *getDirectoryFromIndex(const QModelIndex &index) const {
        if (!index.isValid()) {
            return nullptr;
        }
        auto *info = static_cast<ItemInfo *>(index.internalPointer());
        return static_cast<directory *>(info->ptr);
    }

    file *getFileFromIndex(const QModelIndex &index) const {
        if (!index.isValid()) {
            return nullptr;
        }
        auto *info = static_cast<ItemInfo *>(index.internalPointer());
        return static_cast<file *>(info->ptr);
    }

    string timePointToString(const std::chrono::system_clock::time_point &tp) const {
        // 将 time_point 转换为 time_t
        std::time_t time = std::chrono::system_clock::to_time_t(tp);
        // 创建一个 tm 结构
        std::tm tm = *std::localtime(&time);

        // 使用 stringstream 格式化时间
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};

class mainwindow : public QMainWindow {
    Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = nullptr);
    ~mainwindow() override;

private:
    Ui::mainwindow *ui;
    filesystem *fsX = nullptr;
    FileSystemModel *fsModel = nullptr;
    FileListView *fileListView;

public slots:
    void onCurrentItemChanged(const QModelIndex &current);// 处理当前项变化
    void onFileSelected(const QModelIndex &index);        // 文件选择
    void onFileDoubleClicked(const QModelIndex &index);   // 双击文件
    void onFileRightClicked(const QModelIndex &index);    // 右击文件
    void onSearchTextChanged(const QString &text);        // 处理搜索文本
    void showLoginFailedMessage();                        // 显示登录信息
    void showFileManager();                               // 显示文件管理系统的信号
    void updateDiskCapacity();                            // 更新磁盘容量
    void openButtonCliked();                              // 打开文件按钮按下
    void closeButtonClicked();                            // 关闭文件按钮按下
    void reformatButtonClicked();                         // 格式化按钮按下
    void newUser();                                       // 新建用户
    void deleteUser();                                    // 删除用户
    void newFile();                                       // 新开文件
    void deleteFile();                                    // 删除文件
    void about();                                         //关于
    void newSystem();                                     // 新建系统
    void deleteSystem();                                  // 删除系统
    void exitSystem();                                    // 退出系统

public:
    static vector<fs::path> searchFileSystem();// 搜索当前文件夹下的所有文件系统
    void openFileSystem(QString name);         // 打开文件系统
    void openFileSystem(filesystem *fs);       // 打开文件系统
    void closeFileSystem();                    //关闭文件系统
    void displayFileSystem();                  // 显示文件系统的所有文件
};


// 盘符选择窗口
class DriveSelectionDialog : public QDialog {
    Q_OBJECT

private:
    QComboBox *comboBox;
    string selected;

public:
    void setPath(vector<fs::path> path) {
        for (auto p: path) {
            comboBox->addItem(QString::fromStdString(p.filename().string()).replace(DATA_SUFFIX, "").toUpper());
        }
    }
    string getSelected() {
        return selected;
    }

public:
    DriveSelectionDialog(QWidget *parent = nullptr) : QDialog(parent) {
        // 设置布局和字体
        QVBoxLayout *layout = new QVBoxLayout(this);
        QFont font;
        font.setPointSize(12);

        // 创建一个下拉菜单
        comboBox = new QComboBox(this);
        // 创建一个确认按钮
        QPushButton *okButton = new QPushButton("确认", this);

        // 设置字体
        comboBox->setFont(font);
        okButton->setFont(font);

        // 将下拉菜单和按钮添加到布局中
        layout->addWidget(comboBox);
        layout->addWidget(okButton);

        // 连接确认按钮的点击信号到槽函数
        connect(okButton, &QPushButton::clicked, [this]() {
            QString selectedDrive = comboBox->currentText();
            selected = selectedDrive.toStdString();
            accept();
        });

        setLayout(layout);
        setFixedWidth(200);
        setFixedHeight(120);
        setWindowTitle("请选择要打开的盘符");
    };
};

#endif//FILESYSTEM_MAINWINDOW_H
