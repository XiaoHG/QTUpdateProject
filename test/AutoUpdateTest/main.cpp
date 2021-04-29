#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //主程序开始启动，在调用更新程序之前都将配置文件中的flag设置为false，表示如果没有更新则更新程序不要弹窗
    //flag = false;
    w.WriteConfigFileFlag("0");

    //更新程序启动，检查配置文件的flag如果是false，并且此时检查到没有新版本更新的话，则不弹窗。
    QString strExe = QDir::currentPath() + "/../../src/AutoUpdater/debug/AutoUpdater.exe";
    qDebug() << strExe;
    QProcess::startDetached(strExe);

    return a.exec();
}
