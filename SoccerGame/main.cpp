#include "mainwindow.h"
#include <QtWidgets/QApplication>

// 程序入口，创建并显示主窗口。
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
