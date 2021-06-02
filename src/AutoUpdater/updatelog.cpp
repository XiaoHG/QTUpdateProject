#include "updatelog.h"

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
    QString logPath = QApplication::applicationDirPath() + "/log";
    QDir logDir(logPath);
    if(!logDir.exists())
    {
        logDir.mkdir(logPath);
    }
    QString logName = "updater.log";

    m_fLog.setFileName(logPath + "/" + logName);
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

    QString logMsg = "[";
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
    logMsg.append(current_date);
    logMsg.append("]");
    logMsg.append("[");
    logMsg.append(file + ": " + QString::number(line));
    logMsg.append("]");
    logMsg.append("[");
    logMsg.append(m_listlevel.at(m_level));
    logMsg.append("]: ");
    logMsg.append(msg);
    logMsg.append("\n");
    m_tsLogFile << logMsg;
    m_tsLogFile << flush;

}
