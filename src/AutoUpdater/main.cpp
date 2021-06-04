
#include <QApplication>

#include <QDir>
#include <QTextCodec>
#include <QProcess>
#include <QTextCodec>

#include "updaterui.h"
#include "updater.h"
#include "log.h"

//Global object of update log class.
Log g_log;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    g_log.init();

    //parent process pass argv[1] parameter for from main function or not,
    //if not from main set 0, or others.
    bool _bFromParentMain = false;
    QString _strArg1 = argv[1];
    if(_strArg1 != "0")
    {
        _bFromParentMain = true;
    }
    else
    {
        _bFromParentMain = false;
    }

    //parent process pass argv[2] parameter for choose language,
    //if parent language is english set 0, or others.
    bool _bCh = false;
    QString _strArg2 = argv[2];
    if(_strArg2 != "0")
    {
        _bCh = true;
    }
    else
    {
        _bCh = false;
    }

    QString _parentPid = argv[3];
    //QString parentPid = "1";

    UpdaterUI::getInstance(_bCh)->updater(_bFromParentMain, _parentPid);

    return a.exec();
}
