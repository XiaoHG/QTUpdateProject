#include "xmlparser.h"
#include "log.h"
#include "errorstack.h"

#include <QDomDocument>
#include <QFile>

extern Log g_log;

XMLParser::XMLParser()
{

}

QDomNodeList XMLParser::parseElement(QString xml, QString element)
{
    QDomNodeList _nodeList;
    QFile _file(xml);
    if(!_file.open(QIODevice::ReadOnly | QFile::Text))
    {
        g_log.log(Log::FATAL, "Open XML file for read error: " + _file.errorString(),
                  __FILE__, __LINE__);
        return _nodeList;
    }

    QString _strErrorStr;
    int _iErrorLine;
    int _iErrorColumn;

    QDomDocument _doc;
    if(!_doc.setContent(&_file, false, &_strErrorStr, &_iErrorLine, &_iErrorColumn))
    {
        QString _strErrMsg = xml + QString::asprintf(" Setcontent error: error string = %1, error line = %2, error column = %3")
                .arg(_strErrorStr).arg(_iErrorLine).arg(_iErrorColumn);
        g_log.log(Log::FATAL, _strErrMsg, __FILE__, __LINE__);
        ErrorStack::pushError(XMLPARSE_ERR, g_mapError.find(XMLPARSE_ERR).value());
        _file.close();
        return _nodeList;
    }

    _file.close();
    QDomElement root = _doc.documentElement();
    if(root.tagName() != "autoupdate")
    {
        g_log.log(Log::FATAL, xml + "Xml file is error: root tag is not <autoupdate>", __FILE__, __LINE__);
        return _nodeList;
    }

    _nodeList = root.elementsByTagName(element);

    return _nodeList;
}

