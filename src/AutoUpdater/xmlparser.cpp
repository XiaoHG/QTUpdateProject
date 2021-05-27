#include "xmlparser.h"
#include "updatelog.h"

#include <QDomDocument>
#include <QFile>

extern UpdateLog g_log;

XMLParser::XMLParser()
{

}

QDomNodeList XMLParser::XMLParseElement(QString xml, QString element)
{
    QDomNodeList nodeList;
    QFile file(xml);
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        g_log.log(UpdateLog::FATAL, "Open XML file for read error: " + file.errorString(),
                  __FILE__, __LINE__);
        return nodeList;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if(!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        g_log.log(UpdateLog::FATAL, xml + " Setcontent error", __FILE__, __LINE__);
        file.close();
        return nodeList;
    }

    file.close();
    QDomElement root = doc.documentElement();
    if(root.tagName() != "autoupdate")
    {
        g_log.log(UpdateLog::FATAL, xml + "Xml file is error: root tag is not <autoupdate>", __FILE__, __LINE__);
        return nodeList;
    }

    nodeList = root.elementsByTagName(element);

    return nodeList;
}

void XMLParser::XMLParse(QString xml)
{
    QFile file(xml);
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        g_log.log(UpdateLog::FATAL, "Open XML file for read error: " + file.errorString(),
                  __FILE__, __LINE__);
        return;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if(!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        g_log.log(UpdateLog::FATAL, xml + " Setcontent error", __FILE__, __LINE__);
        file.close();
        return;
    }

    file.close();
    QDomElement root = doc.documentElement();
    if(root.tagName() != "autoupdate")
    {
        g_log.log(UpdateLog::FATAL, xml + "Xml file is error: root tag is not <autoupdate>", __FILE__, __LINE__);
        return;
    }

    QDomNodeList nodeList = root.elementsByTagName("file");
    for(int i = 0; i < nodeList.size(); ++i)
    {
        QString name = nodeList.at(i).toElement().attribute("name");
        QString dir = nodeList.at(i).toElement().attribute("dir");
        QString version = nodeList.at(i).toElement().attribute("version");
        g_log.log(UpdateLog::FATAL, name + ", " + dir + ", " + version, __FILE__, __LINE__);
    }
    nodeList = root.elementsByTagName("version");
    for(int i = 0; i < nodeList.size(); ++i)
    {
        g_log.log(UpdateLog::FATAL, "Version = " + nodeList.at(i).toElement().text(), __FILE__, __LINE__);
    }

}

