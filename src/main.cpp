#include "QApplication"
#include "data.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    // 初始化数据
    if (!initData()) {
        cerr << "Fail to init data.";
    }

    QApplication a(argc, argv);
    // 运行主窗口
    mainwindow mw;
    mw.show();
    QApplication::exec();

    // 窗口被关闭后
    closeData();
    return 0;
}
