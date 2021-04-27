#include "cxmlparser.h"

#include <QDomDocument>
#include <QFile>
#include <QDebug>

CXMLParser::CXMLParser()
{

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
    if(root.tagName() != "filelist")
    {
        qDebug() << "root.tagname != filelist";
    }
    else
    {
        QDomNodeList nodeList = root.elementsByTagName("file");
        for(int i = 0; i < nodeList.size(); ++i)
        {
            qDebug() << nodeList.at(i).toElement().attribute("name")
                     << nodeList.at(i).toElement().attribute("dir")
                     << nodeList.at(i).toElement().attribute("version");
        }
    }
}

/**
 * @brief CXMLParser::XMLParseVersion
 * @param xml
 * 解析获取版本控制文件中版本号
 */
QString CXMLParser::XMLParseVersion(QString xml)
{
    return "";
}
