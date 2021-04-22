#include "mainwindow.h"
#include <QApplication>
#include "updateclientui.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    //UpadateUI
    UpdateClientUI::getInstall()->testUpdate(true);
    UpdateClientUI::getInstall()->checkUpdate();

    return a.exec();
}
