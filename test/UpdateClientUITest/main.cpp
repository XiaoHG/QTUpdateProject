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
    UpdateClientUI *update = UpdateClientUI::getInstall();
    qDebug() << update->checkUpdate();
    if(update->checkUpdate())
        update->show();

    return a.exec();
}
