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

    w.StartUpdateProcess("1", "0");

    return a.exec();
}
