#ifndef CXMLPARSER_H
#define CXMLPARSER_H

#include <QDomDocument>
#include <QString>

class XMLParser
{
public:
    XMLParser();
    static QDomNodeList parseElement(QString xml, QString element);
    static void parse(QString xml);
    static QString parseVersion(QString xml);
};

#endif // CXMLPARSER_H
