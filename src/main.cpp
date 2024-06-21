#include "QApplication"
#include "filesystem.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    // 调试区
    char *tmp = new char[65];
    cout << sizeof(tmp) << endl;
    cout << sizeof(*tmp);
    filesystem fs("data", 128 * 1024 * 1024, 512);
    // 调试区结束

    QApplication a(argc, argv);
    // 运行主窗口
    mainwindow mw;
    mw.show();
    QApplication::exec();
    return 0;
}
