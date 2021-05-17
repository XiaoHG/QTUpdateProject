#include "cautoupdater.h"
#include "cxmlparser.h"
#include <QApplication>
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CAutoUpdater w;
    w.show();

    //w.downloadXMLFile(); //拉取服务器updater.xml成功
    QString currentDir = QDir::currentPath();
    QString clientXMLPath = currentDir + "/updater.xml";
    qDebug() << "clientXMLPath = " << clientXMLPath;
    QString downloadXMLPath = currentDir + "/download/updater.xml";
    qDebug() << "downloadXMLPath = " << downloadXMLPath;
    CXMLParser parse;
    qDebug() << "没有需要更新的文件！";
    parse.XMLParse(downloadXMLPath);

    qDebug() << "没有需要更新的文件！2";
    w.CheckUpdateFiles(downloadXMLPath, clientXMLPath);
    w.DownloadUpdateFiles();

    return a.exec();
}
