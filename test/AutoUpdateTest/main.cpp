#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QStringList arguments;
    arguments << "1" << "2" << "3" << "4";
    //更新程序启动，检查配置文件的flag如果是false，并且此时检查到没有新版本更新的话，则不弹窗。
    QString strExe = QApplication::applicationDirPath() + "/../../../src/AutoUpdater/debug/AutoUpdater0.0.exe";
    qDebug() << strExe;
    QProcess::startDetached(strExe, arguments);


    return a.exec();
}
