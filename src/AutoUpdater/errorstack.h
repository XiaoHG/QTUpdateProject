#ifndef ERRORSTACK_H
#define ERRORSTACK_H

#include <QObject>
#include <QVector>

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

class ErrorStack : public QObject
{
    Q_OBJECT

public:
    explicit ErrorStack(QObject *parent = nullptr);

    static void pushError(const int errCode, const QString errStr);
    static QString getErrorString(const int errCode);
    static const QVector<PERROR_STRUCT>& getErrorStack();

signals:

public slots:

private:
    static QVector<PERROR_STRUCT> m_vectorErrorStack;
};

#endif // ERRORSTACK_H
