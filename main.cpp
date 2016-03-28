#include <QApplication> // 后面会创建QApplication类型的对象
#include "mainwindow.h"

int main(int argc, char *argv[])    // 支持命令行参数
{
    QApplication app(argc, argv);   // 命令行参数传递给对象appa, 构造函数需要
    MainWindow mainWin;
    mainWin.show();
    return app.exec(); // 处理事件和回收内存
}

