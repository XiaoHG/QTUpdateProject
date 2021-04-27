#ifndef CXMLPARSER_H
#define CXMLPARSER_H

#include <QString>



class CXMLParser
{
public:
    CXMLParser();
    static void XMLParse(QString xml);
    static QString XMLParseVersion(QString xml);
};

#endif // CXMLPARSER_H
