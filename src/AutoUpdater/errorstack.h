#ifndef ERRORSTACK_H
#define ERRORSTACK_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QPair>

typedef struct ErrorStruct
{
    ErrorStruct(int code, QString str)
    {
        iErrCode = code;
        strError = str;
    }

    int iErrCode;
    QString strError;
}ERROR_STRUCT, *PERROR_STRUCT;

typedef enum ErrorEnum{
    PARSE_DOWNLOAD_XML_ERR = 1024,
    PARSE_LOCAL_XML_ERR,
    OPEN_LOCAL_VERSIONINFO_FILE_ERR,
    OPEN_DOWNLOAD_VERSIONINFO_FILE_ERR,
    OPEN_INITUPDATER_FILE_ERR,
    OPEN_SCRIPT_FILE_ERR,
    MD5_CHECK_ERR,
    DOWNLOAD_TIMEOUT,
    RETRYDOWNLOAD_TIMESOVER,
    XMLPARSE_ERR
}ERROR_ENUM;

//error pair list, for client view
const static std::initializer_list<std::pair<ERROR_ENUM, QString>> s_list = {
    std::pair<ERROR_ENUM, QString>(PARSE_DOWNLOAD_XML_ERR, QObject::tr("Parse server xml file error.")),
    std::pair<ERROR_ENUM, QString>(PARSE_LOCAL_XML_ERR, QObject::tr("Parse local xml file error.")),
    std::pair<ERROR_ENUM, QString>(OPEN_LOCAL_VERSIONINFO_FILE_ERR, QObject::tr("Open local version information file error.")),
    std::pair<ERROR_ENUM, QString>(OPEN_DOWNLOAD_VERSIONINFO_FILE_ERR, QObject::tr("Open download version information file error.")),
    std::pair<ERROR_ENUM, QString>(OPEN_INITUPDATER_FILE_ERR, QObject::tr("Open init updater xml version information file error.")),
    std::pair<ERROR_ENUM, QString>(OPEN_SCRIPT_FILE_ERR, QObject::tr("Open script file error.")),
    std::pair<ERROR_ENUM, QString>(MD5_CHECK_ERR, QObject::tr("Download file content error.")),
    std::pair<ERROR_ENUM, QString>(DOWNLOAD_TIMEOUT, QObject::tr("Download time out.")),
    std::pair<ERROR_ENUM, QString>(RETRYDOWNLOAD_TIMESOVER, QObject::tr("Download retry over.")),
    std::pair<ERROR_ENUM, QString>(XMLPARSE_ERR, QObject::tr("Xml file parse error."))
};
//for out side, client.
const QMap<ERROR_ENUM, QString> g_mapError(s_list);

class ErrorStack : public QObject
{
    Q_OBJECT

public:
    explicit ErrorStack(QObject *parent = nullptr);
    virtual ~ErrorStack();

    static void pushError(const int errCode, const QString errStr);
    static QString getErrorString(const int errCode);
    static const QVector<PERROR_STRUCT>& getErrorStack();
signals:

public slots:

private:
    static QVector<PERROR_STRUCT> m_vectorErrorStack;
};

#endif // ERRORSTACK_H
