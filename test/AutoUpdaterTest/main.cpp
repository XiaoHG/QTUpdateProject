#include "mainwindow.h"
#include <QApplication>
#include "autoupdaterui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    AutoUpdaterUI::getInstance();

    //主程序开始启动，在调用更新程序之前都将配置文件中的flag设置为false，表示如果没有更新则更新程序不要弹窗
    //flag = false;
//    w.WriteConfigFileFlag("0");
//    AutoUpdaterUI::getInstance()->CheckUpdate();

    return a.exec();
}
