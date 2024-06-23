#include "QApplication"
#include "login.h"
#include "mainwindow.h"
#include "reformat.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    login re;
    re.show();
    mainwindow mw;
    mw.show();
    QApplication::exec();
    return 0;
}
