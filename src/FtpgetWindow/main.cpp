#include "FtpgetWindow.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FtpgetWindow w;
    w.show();

    return a.exec();
}
