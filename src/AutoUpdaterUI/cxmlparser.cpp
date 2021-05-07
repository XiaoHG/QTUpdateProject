#include "cxmlparser.h"

#include <QDomDocument>
#include <QFile>
#include <QDebug>

CXMLParser::CXMLParser()
{

}

QDomNodeList CXMLParser::XMLParseElement(QString xml, QString element)
{
    QDomNodeList nodeList;
    QFile file(xml);
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        qDebug() << "Open XML file for read error ... " << file.errorString();
        return nodeList;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if(!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << xml + " Setcontent error ... ";
        qDebug() << errorStr;
        qDebug() << errorLine;
        qDebug() << errorColumn;
        file.close();
        return nodeList;
    }

    file.close();
    QDomElement root = doc.documentElement();
    if(root.tagName() != "autoupdate")
    {
        qDebug() << QStringLiteral("xml文件格式错误！");
        return nodeList;
    }

    nodeList = root.elementsByTagName(element);

    return nodeList;
}

void CXMLParser::XMLParse(QString xml)
{
    QFile file(xml);
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        qDebug() << "Open XML file for read error ... ";
        return;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if(!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "Setcontent error ... ";
        qDebug() << errorStr;
        qDebug() << errorLine;
        qDebug() << errorColumn;
        file.close();
        return;
    }

    file.close();
    QDomElement root = doc.documentElement();
    if(root.tagName() != "autoupdate")
    {
        qDebug() << QStringLiteral("xml文件可是错误！");
        return;
    }

    QDomNodeList nodeList = root.elementsByTagName("file");
    for(int i = 0; i < nodeList.size(); ++i)
    {
        qDebug() << nodeList.at(i).toElement().attribute("name")
                 << nodeList.at(i).toElement().attribute("dir")
                 << nodeList.at(i).toElement().attribute("version");
    }
    nodeList = root.elementsByTagName("version");
    for(int i = 0; i < nodeList.size(); ++i)
    {
        qDebug() << "Version = " << nodeList.at(i).toElement().text();
    }

}

