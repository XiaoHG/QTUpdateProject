
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QTextCodec>
#include <QProcess>

#include "autoupdaterui.h"
#include "autoupdater.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Delete old version.
//    QProcess p;
//    p.start(QApplication::applicationDirPath() + "/del.bat");

    QTextCodec *codec = QTextCodec::codecForLocale();
    QTextCodec::setCodecForLocale(codec);

    bool bFirst;
    QString strArg1 = argv[1];
    if(strArg1 == "1")
    {
        bFirst = true;
    }
    else
    {
        bFirst = false;
    }

    AutoUpdaterUI::getInstance()->CheckUpdater(bFirst);

    return a.exec();
}
