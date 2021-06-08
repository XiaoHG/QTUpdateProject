#ifndef GLOBALDEF_HPP
#define GLOBALDEF_HPP

#include "loger/log.h"

#include <QFile>

//Global object of update log class.
Log g_log;

#ifndef LOADQSS
#define LOADQSS(qssFile)                        \
{                                               \
    QFile file(qssFile);                        \
    file.open(QFile::ReadOnly);                 \
    if(file.isOpen())                           \
    {                                           \
        qApp->setStyleSheet(file.readAll());    \
        file.close();                           \
    }                                           \
}
#endif

#ifndef SAFEDELETE
#define SAFEDELETE(pointer) \
{                           \
    if(pointer)             \
{                       \
    delete pointer;     \
    }                       \
    pointer = nullptr;          \
    }
#endif


namespace GlobalSpace
{
    const QString STYLE_QSS_FILE_PATH = ":/stylesheet/stylesheet.qss";
}

#endif // GLOBALDEF_HPP
