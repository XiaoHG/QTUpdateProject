#include "cautoupdater.h"
#include <QApplication>
#include <QDir>
#include <QDebug>
#include "cupdateclientui.h"
#include "cxmlparser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CUpdateClientUI::getInstance()->CheckUpdate();

    return a.exec();
}
