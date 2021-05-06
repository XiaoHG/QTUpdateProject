﻿#include "mainwindow.h"
#include "updateclientui.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    //UpadateUI
    CUpdateClientUI::getInstance()->CheckUpdate();

    return a.exec();
}
