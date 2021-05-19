
#include <QApplication>
#include <QDir>
#include <QDebug>

#include "autoupdaterui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AutoUpdaterUI::getInstance()->CheckUpdater(true);

    return a.exec();
}
