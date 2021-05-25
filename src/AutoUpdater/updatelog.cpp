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
    if(m_logFile.isOpen())
    {
        m_logFile.close();
    }
}

void UpdateLog::Init()
{
    //Default log level is INFO
    m_level = INFO;

    m_levelStringList.append("ALL");
    m_levelStringList.append("TRACE");
    m_levelStringList.append("DEBUG");
    m_levelStringList.append("INFO");
    m_levelStringList.append("WARN");
    m_levelStringList.append("ERROR");
    m_levelStringList.append("FATAL");
    m_levelStringList.append("OFF");

    //open log file
    QString logPath = QApplication::applicationDirPath() + "/log";
    QDir logDir(logPath);
    if(!logDir.exists())
    {
        logDir.mkdir(logPath);
    }
    QString logName = "updater.log";

    m_logFile.setFileName(logPath + "/" + logName);
    if(!m_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        return;
    }

    m_logTextStream.setDevice(&m_logFile);
}

void UpdateLog::SetLoglevel(ELOGLEVEL level)
{
    m_level = level;
}

void UpdateLog::log(ELOGLEVEL level, const QString msg, QString file, int line)
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
    logMsg.append(m_levelStringList.at(m_level));
    logMsg.append("]: ");
    logMsg.append(msg);
    logMsg.append("\n");
    m_logTextStream << logMsg;
    m_logTextStream << flush;

}
