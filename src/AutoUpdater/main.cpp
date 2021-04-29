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

//    CXMLParser parse;
//    parse.XMLParse("./download/updater.xml");

//    CAutoUpdater abc;
//    abc.makeXML(QDir::currentPath() + "/123.xml");

    return a.exec();
}
