﻿#include "updatelog.h"

#include <QApplication>
#include <QDir>
#include <QDateTime>

UpdateLog::UpdateLog(QObject *parent)
    : QObject(parent)
{

}

UpdateLog::~UpdateLog()
{
    if(m_fLog.isOpen())
    {
        m_fLog.close();
    }
}

void UpdateLog::init()
{
    //Default log level is INFO
    m_level = INFO;

    m_listlevel.append("ALL");
    m_listlevel.append("TRACE");
    m_listlevel.append("DEBUG");
    m_listlevel.append("INFO");
    m_listlevel.append("WARN");
    m_listlevel.append("ERROR");
    m_listlevel.append("FATAL");
    m_listlevel.append("OFF");

    //open log file
    QString _strLogPath = QApplication::applicationDirPath() + "/log";
    QDir _dir(_strLogPath);
    if(!_dir.exists())
    {
        _dir.mkdir(_strLogPath);
    }
    QString _strFileName = "updater.log";

    m_fLog.setFileName(_strLogPath + "/" + _strFileName);
    if(!m_fLog.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        return;
    }

    m_tsLogFile.setDevice(&m_fLog);
}

void UpdateLog::setLoglevel(ELOGLEVEL level)
{
    m_level = level;
}

void UpdateLog::log(const ELOGLEVEL level, const QString &msg,
                    const QString &file, const int line)
{
    if(level < m_level)
    {
        return;
    }

    QString _strLogMsg = "[";
    QDateTime _dtCurrent = QDateTime::currentDateTime();
    QString _strCurrentData = _dtCurrent.toString("yyyy.MM.dd hh:mm:ss.zzz");
    _strLogMsg.append(_strCurrentData);
    _strLogMsg.append("]");
    _strLogMsg.append("[");
    _strLogMsg.append(file + ": " + QString::number(line));
    _strLogMsg.append("]");
    _strLogMsg.append("[");
    _strLogMsg.append(m_listlevel.at(m_level));
    _strLogMsg.append("]: ");
    _strLogMsg.append(msg);
    _strLogMsg.append("\n");
    m_tsLogFile << _strLogMsg;
    m_tsLogFile << flush; //flush new

}
