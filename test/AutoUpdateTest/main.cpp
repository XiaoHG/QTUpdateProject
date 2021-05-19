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

    //更新程序启动，检查配置文件的flag如果是false，并且此时检查到没有新版本更新的话，则不弹窗。
    QString strExe = QApplication::applicationDirPath() + "/../../../src/AutoUpdater/debug/AutoUpdater.exe";
    qDebug() << strExe;
    QProcess::startDetached(strExe);

    return a.exec();
}
