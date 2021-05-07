#ifndef CXMLPARSER_H
#define CXMLPARSER_H

#include <QDomDocument>
#include <QString>

class CXMLParser
{
public:
    CXMLParser();
    static QDomNodeList XMLParseElement(QString xml, QString element);
    static void XMLParse(QString xml);
    static QString XMLParseVersion(QString xml);
};

#endif // CXMLPARSER_H
