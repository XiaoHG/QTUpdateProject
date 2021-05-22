
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QTextCodec>
#include <QProcess>
#include <QTextCodec>

#include "autoupdaterui.h"
#include "autoupdater.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool bFirst;
    //parent process pass argv[1] parameter for from main function or not,
    //if not from main set 0, or others.
    QString strArg1 = argv[1];
    if(strArg1 != "0")
    {
        bFirst = true;
    }
    else
    {
        bFirst = false;
    }

    bool bCh = true;
    //parent process pass argv[1] parameter for choose language,
    //if parent language is english set 0, or others.s
//    QString strArg2 = argv[2];
//    if(strArg2 != "0")
//    {
//        bCh = true;
//    }
//    else
//    {
//        bCh = false;
//    }

    //AutoUpdaterUI::getInstance()->Language(bCh);
    AutoUpdaterUI::getInstance(bCh)->CheckUpdater(bFirst);


    return a.exec();
}
