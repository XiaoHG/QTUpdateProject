#include "cautoupdater.h"
#include <QApplication>
#include <QDir>
#include <QDebug>
#include "cupdateclientui.h"
#include "cxmlparser.h"
#include "cxmlparser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CUpdateClientUI::getInstance()->CheckUpdate();

//    CXMLParser parse;
//    parse.XMLParse("./download/updater.xml");

    return a.exec();
}
