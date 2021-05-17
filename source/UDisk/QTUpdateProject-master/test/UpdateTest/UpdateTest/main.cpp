#include "mainwindow.h"
#include <QApplication>
#include <QProcess>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    if(QProcess::startDetached("../../../src/Updater/debug/Updater.exe"))
    {
        qDebug() << "main exit";
        exit(0);
    }
    qDebug() << "main continue";

    return a.exec();
}
