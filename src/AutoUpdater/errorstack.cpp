#include "errorstack.h"
#include "log.h"

extern Log g_log;

QVector<PERROR_STRUCT> ErrorStack::m_vectorErrorStack = {};

ErrorStack::ErrorStack(QObject *parent)
    : QObject(parent)
{

}

ErrorStack::~ErrorStack()
{
    for(int i = 0; i < m_vectorErrorStack.size(); i++)
    {
        PERROR_STRUCT _sTmpErr = m_vectorErrorStack.at(i);
        if(_sTmpErr)
        {
            delete _sTmpErr;
        }
    }
}

void ErrorStack::pushError(const int errCode, const QString errStr)
{
    PERROR_STRUCT _sTmpErr = new ERROR_STRUCT(errCode, errStr);
    m_vectorErrorStack.push_back(_sTmpErr);
}

QString ErrorStack::getErrorString(const int errCode)
{
    QString _strError;
    for(int i = 0; i < m_vectorErrorStack.size(); i++)
    {
        if(m_vectorErrorStack.at(i)->iErrCode == errCode)
        {
            _strError = m_vectorErrorStack.at(i)->strError;
        }
    }
    return _strError;
}

const QVector<PERROR_STRUCT> &ErrorStack::getErrorStack()
{
    return m_vectorErrorStack;
}
