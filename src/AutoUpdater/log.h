#ifndef UPDATELOG_H
#define UPDATELOG_H

#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QWidget>

class Log : public QObject
{
    Q_OBJECT

public:
    typedef enum eLogLevel{
        ALL,
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        OFF
    }ELOGLEVEL;

public:
    explicit Log(QObject *parent = nullptr);
    virtual ~Log();

    void log(const ELOGLEVEL level, const QString &msg,
             const QString &file = "test", const int line = 0);

    void setLoglevel(ELOGLEVEL level);

    void init();

signals:

public slots:

private:
    //handle
    QFile       m_fLog;
    QTextStream m_tsLogFile;

    //level
    ELOGLEVEL   m_level;
    QStringList m_listlevel;

};

#endif // UPDATELOG_H
