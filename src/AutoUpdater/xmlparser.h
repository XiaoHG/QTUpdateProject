#ifndef CXMLPARSER_H
#define CXMLPARSER_H

#include <QDomDocument>
#include <QString>

class XMLParser
{
public:
    XMLParser();
    static QDomNodeList parseElement(QString xml, QString element);
};

#endif // CXMLPARSER_H
