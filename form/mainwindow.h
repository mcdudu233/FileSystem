//
// Created by dudu233 on 2024/6/17.
//

#ifndef FILESYSTEM_MAINWINDOW_H
#define FILESYSTEM_MAINWINDOW_H

#include "filesystem.h"
#include "login.h"
#include "reformat.h"
#include "textedit.h"
#include <QAction>
#include <QBrush>
#include <QCloseEvent>
#include <QColor>
#include <QComboBox>
#include <QFormLayout>
#include <QHash>
#include <QInputDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QModelIndex>
#include <QPushButton>
#include <QRegularExpression>
#include <QVBoxLayout>
#include <QVector>


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
    QVector<QPersistentModelIndex> searchResults;// 使用 QVector 存储匹配结果的索引集合
    enum class ItemType { Directory,
                          File };
    typedef struct {
        void *ptr;
        ItemType type;
    } ItemInfo;
    mutable QHash<void *, ItemInfo *> itemInfoMap = {};// 使用 QHash 存储 ItemInfo 对象

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
        return 4;
    }

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
    }

    Qt::ItemFlags flags(const QModelIndex &index) const override {
        if (!index.isValid()) {
            return Qt::NoItemFlags;
        }
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }

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
    }

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
            itemInfoMap.insert(childDir, info);
            return createIndex(row, column, info);
        } else {
            file *childFile = &parentDir->getFiles()->at(row - parentDir->getDirectories()->size());
            auto *info = new ItemInfo{static_cast<void *>(childFile), ItemType::File};
            itemInfoMap.insert(childFile, info);
            return createIndex(row, column, info);
        }
    }

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
                if (*parentDir == *root) {
                    return {};
                }
                int row = 0;
                // 计算行号
                directory *grandDir = fsX->getFatherByName(*parentDir);
                if (grandDir) {
                    vector<directory> dirs = *grandDir->getDirectories();
                    for (; row < dirs.size(); row++) {
                        if (*parentDir == dirs[row]) {
                            break;
                        }
                    }
                }
                return createIndex(row, 0, itemInfoMap[parentDir]);
            }
        } else {
            file *childFile = getFileFromIndex(index);
            if (!childFile) {
                return {};
            }

            directory *parentDir = fsX->getFatherByName(*childFile);
            if (parentDir) {
                if (*parentDir == *root) {
                    return {};
                }
                int row = 0;
                // 计算行号
                directory *grandDir = fsX->getFatherByName(*parentDir);
                if (grandDir) {
                    vector<directory> dirs = *grandDir->getDirectories();
                    for (; row < dirs.size(); row++) {
                        if (*parentDir == dirs[row]) {
                            break;
                        }
                    }
                }
                return createIndex(row, 0, itemInfoMap[parentDir]);
            }
        }
        return {};
    }

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
    }

public:
    /* 工具 */
    static bool isDirectory(const QModelIndex &index) {
        if (!index.isValid()) {
            return false;
        }
        auto *info = static_cast<ItemInfo *>(index.internalPointer());
        return info->type == ItemType::Directory;
    }

    static directory *getDirectoryFromIndex(const QModelIndex &index) {
        if (!index.isValid()) {
            return nullptr;
        }
        auto *info = static_cast<ItemInfo *>(index.internalPointer());
        return static_cast<directory *>(info->ptr);
    }

    static file *getFileFromIndex(const QModelIndex &index) {
        if (!index.isValid()) {
            return nullptr;
        }
        auto *info = static_cast<ItemInfo *>(index.internalPointer());
        return static_cast<file *>(info->ptr);
    }

    static string timePointToString(const std::chrono::system_clock::time_point &tp) {
        // 将 time_point 转换为 time_t
        std::time_t time = std::chrono::system_clock::to_time_t(tp);
        // 创建一个 tm 结构
        std::tm tm = *std::localtime(&time);

        // 使用 stringstream 格式化时间
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
    void addSearchResult(const QModelIndex &index);
    void clearSearchResults();
};

// 主窗口类
class mainwindow : public QMainWindow {
    Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = nullptr);
    ~mainwindow() override;
    void closeEvent(QCloseEvent *event) override;// 关闭事件重写

private:
    Ui::mainwindow *ui;
    filesystem *fsX = nullptr;
    FileSystemModel *fsModel = nullptr;

public slots:
    void onCustomContextMenuRequested(const QPoint &pos); // treeView的右键菜单
    void onCurrentItemChanged(const QModelIndex &current);// 处理当前项变化
    // 处理搜索文本
    /* 系统操作 */
    void openSystem();    // 打开系统按钮按下
    void closeSystem();   // 关闭系统按钮按下
    void reformatSystem();// 格式化按钮按下
    void deleteSystem();  // 删除文件系统
    void exitSystem();    // 退出系统
    /* 用户操作 */
    void newUser();     // 新建用户
    void deleteUser();  // 删除用户
    void loginUser();   // 登录用户
    void changeUser();  // 切换用户
    void passwordUser();// 修改用户密码
    /* 文件操作 */
    void newFile();   // 新文件
    void deleteFile();// 删除文件
    void openFile();  // 打开文件
    void renameFile();//文件重命名
    void fileNature();// 文件属性
    /* 目录操作 */
    void openDirectory();  // 打开目录
    void newDirectory();   // 新文件夹
    void deleteDirectory();// 删除目录
    void renameDirectory();// 重命名目录
    void directoryNature();// 目录属性
    /*其他操作*/
    void about();// 关于
    void onSearchTextChanged(const QString &text);
    void expandParents(const QModelIndex &index);
    bool filterAndExpandTreeView(const QModelIndex &index, const QRegularExpression &regExp);


public:
    static string getSizeString(float f);                                           // 格式化文件大小
    static string getTimeString(const std::chrono::system_clock::time_point &tp);   // 格式化时间
    static vector<fs::path> searchFileSystem();                                     // 搜索当前文件夹下的所有文件系统
    void openFileSystem(QString name);                                              // 打开文件系统
    void openFileSystem(filesystem *fs);                                            // 打开文件系统
    void closeFileSystem();                                                         // 关闭文件系统
    void displayFileSystem();                                                       // 显示文件系统的所有文件
    void updateDiskCapacity();                                                      // 更新磁盘容量
    bool isOpened();                                                                // 检测文件系统是否已经打开
    bool filterTreeView(const QModelIndex &index, const QRegularExpression &regExp);// 显示查找结果
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
        setWindowTitle("文件系统管理器");
        // 设置窗口图标
        QIcon icon("./res/main.png");
        if (icon.isNull()) {
            icon.addFile("../res/main.png");
        }
        setWindowIcon(icon);

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
