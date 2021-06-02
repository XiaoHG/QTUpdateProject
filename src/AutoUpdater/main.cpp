
#include <QApplication>

#include <QDir>
#include <QTextCodec>
#include <QProcess>
#include <QTextCodec>

#include "autoupdaterui.h"
#include "autoupdater.h"
#include "updatelog.h"

//Global object of update log class.
UpdateLog g_log;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    g_log.init();

    //parent process pass argv[1] parameter for from main function or not,
    //if not from main set 0, or others.
    bool isFromParentMain = false;
    QString strArg1 = argv[1];
    if(strArg1 != "0")
    {
        isFromParentMain = true;
    }
    else
    {
        isFromParentMain = false;
    }

    //parent process pass argv[2] parameter for choose language,
    //if parent language is english set 0, or others.
    bool bCh = false;
    QString strArg2 = argv[2];
    if(strArg2 != "0")
    {
        bCh = true;
    }
    else
    {
        bCh = false;
    }

    QString parentPid = argv[3];
	//QString parentPid = "1";

    AutoUpdaterUI::getInstance(bCh)->updater(isFromParentMain, parentPid);

    return a.exec();
}
