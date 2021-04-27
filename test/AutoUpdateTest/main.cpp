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

    QString strExe = QDir::currentPath() + "/../../src/AutoUpdater/debug/AutoUpdater.exe";
    qDebug() << strExe;
    QProcess::startDetached(strExe);

    //QFile::copy(QDir::currentPath() + "/versionInfo.txt", QDir::currentPath() + "/versionInfo11111111.txt");

    return a.exec();
}
