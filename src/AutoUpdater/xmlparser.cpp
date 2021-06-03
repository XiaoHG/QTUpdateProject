#include "xmlparser.h"
#include "updatelog.h"

#include <QDomDocument>
#include <QFile>

extern UpdateLog g_log;

XMLParser::XMLParser()
{

}

QDomNodeList XMLParser::parseElement(QString xml, QString element)
{
    QDomNodeList _nodeList;
    QFile _file(xml);
    if(!_file.open(QIODevice::ReadOnly | QFile::Text))
    {
        g_log.log(UpdateLog::FATAL, "Open XML file for read error: " + _file.errorString(),
                  __FILE__, __LINE__);
        return _nodeList;
    }

    QString _strErrorStr;
    int _iErrorLine;
    int _iErrorColumn;

    QDomDocument _doc;
    if(!_doc.setContent(&_file, false, &_strErrorStr, &_iErrorLine, &_iErrorColumn))
    {
        g_log.log(UpdateLog::FATAL, xml + " Setcontent error", __FILE__, __LINE__);
        _file.close();
        return _nodeList;
    }

    _file.close();
    QDomElement root = _doc.documentElement();
    if(root.tagName() != "autoupdate")
    {
        g_log.log(UpdateLog::FATAL, xml + "Xml file is error: root tag is not <autoupdate>", __FILE__, __LINE__);
        return _nodeList;
    }

    _nodeList = root.elementsByTagName(element);

    return _nodeList;
}

void XMLParser::parse(QString xml)
{
    QFile _file(xml);
    if(!_file.open(QIODevice::ReadOnly | QFile::Text))
    {
        g_log.log(UpdateLog::FATAL, "Open XML file for read error: " + _file.errorString(),
                  __FILE__, __LINE__);
        return;
    }

    QString _strErrorStr;
    int _iErrorLine;
    int _iErrorColumn;

    QDomDocument _doc;
    if(!_doc.setContent(&_file, false, &_strErrorStr, &_iErrorLine, &_iErrorColumn))
    {
        g_log.log(UpdateLog::FATAL, xml + " Setcontent error", __FILE__, __LINE__);
        _file.close();
        return;
    }

    _file.close();
    QDomElement _root = _doc.documentElement();
    if(_root.tagName() != "autoupdate")
    {
        g_log.log(UpdateLog::FATAL, xml + "Xml file is error: root tag is not <autoupdate>", __FILE__, __LINE__);
        return;
    }

    QDomNodeList _nodeList = _root.elementsByTagName("file");
    for(int i = 0; i < _nodeList.size(); ++i)
    {
        QString _strName = _nodeList.at(i).toElement().attribute("name");
        QString _strDir = _nodeList.at(i).toElement().attribute("dir");
        QString _strVersion = _nodeList.at(i).toElement().attribute("version");
        g_log.log(UpdateLog::FATAL, _strName + ", " + _strDir + ", " + _strVersion, __FILE__, __LINE__);
    }
    _nodeList = _root.elementsByTagName("version");
    for(int i = 0; i < _nodeList.size(); ++i)
    {
        g_log.log(UpdateLog::FATAL, "Version = " + _nodeList.at(i).toElement().text(), __FILE__, __LINE__);
    }

}

