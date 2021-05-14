#ifndef CXMLPARSER_H
#define CXMLPARSER_H

#include <QDomDocument>
#include <QString>

class XMLParser
{
public:
    XMLParser();
    static QDomNodeList XMLParseElement(QString xml, QString element);
    static void XMLParse(QString xml);
    static QString XMLParseVersion(QString xml);
};

#endif // CXMLPARSER_H
