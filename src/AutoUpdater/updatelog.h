#ifndef UPDATELOG_H
#define UPDATELOG_H

#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QWidget>

class UpdateLog : public QObject
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
    explicit UpdateLog(QObject *parent = nullptr);
    virtual ~UpdateLog();

    void log(ELOGLEVEL level, const QString msg, QString file = "test", int line = 0);

    void SetLoglevel(ELOGLEVEL level);
    void Init();
signals:

public slots:

private:
    //handle
    QFile m_logFile;
    QTextStream m_logTextStream;

    //level
    ELOGLEVEL m_level;
    QStringList m_levelStringList;

};

#endif // UPDATELOG_H
