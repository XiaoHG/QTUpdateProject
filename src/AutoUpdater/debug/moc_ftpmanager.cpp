/****************************************************************************
** Meta object code from reading C++ file 'ftpmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ftpmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ftpmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FtpManager_t {
    QByteArrayData data[13];
    char stringdata0[258];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FtpManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FtpManager_t qt_meta_stringdata_FtpManager = {
    {
QT_MOC_LITERAL(0, 0, 10), // "FtpManager"
QT_MOC_LITERAL(1, 11, 24), // "signal_downloadXmlFinish"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 23), // "signal_downloadChFinish"
QT_MOC_LITERAL(4, 61, 23), // "signal_downloadEnFinish"
QT_MOC_LITERAL(5, 85, 27), // "signal_startDownloadPerFile"
QT_MOC_LITERAL(6, 113, 28), // "signal_finishDownloadPerFile"
QT_MOC_LITERAL(7, 142, 17), // "signal_replyError"
QT_MOC_LITERAL(8, 160, 23), // "on_reply_downloadFinish"
QT_MOC_LITERAL(9, 184, 20), // "on_timer_downloading"
QT_MOC_LITERAL(10, 205, 14), // "on_reply_error"
QT_MOC_LITERAL(11, 220, 27), // "QNetworkReply::NetworkError"
QT_MOC_LITERAL(12, 248, 9) // "errorCode"

    },
    "FtpManager\0signal_downloadXmlFinish\0"
    "\0signal_downloadChFinish\0"
    "signal_downloadEnFinish\0"
    "signal_startDownloadPerFile\0"
    "signal_finishDownloadPerFile\0"
    "signal_replyError\0on_reply_downloadFinish\0"
    "on_timer_downloading\0on_reply_error\0"
    "QNetworkReply::NetworkError\0errorCode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FtpManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    0,   61,    2, 0x06 /* Public */,
       5,    1,   62,    2, 0x06 /* Public */,
       6,    1,   65,    2, 0x06 /* Public */,
       7,    1,   68,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   71,    2, 0x09 /* Protected */,
       9,    0,   72,    2, 0x09 /* Protected */,
      10,    1,   73,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,

       0        // eod
};

void FtpManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FtpManager *_t = static_cast<FtpManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signal_downloadXmlFinish(); break;
        case 1: _t->signal_downloadChFinish(); break;
        case 2: _t->signal_downloadEnFinish(); break;
        case 3: _t->signal_startDownloadPerFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->signal_finishDownloadPerFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->signal_replyError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->on_reply_downloadFinish(); break;
        case 7: _t->on_timer_downloading(); break;
        case 8: _t->on_reply_error((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FtpManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::signal_downloadXmlFinish)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FtpManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::signal_downloadChFinish)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FtpManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::signal_downloadEnFinish)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (FtpManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::signal_startDownloadPerFile)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (FtpManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::signal_finishDownloadPerFile)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (FtpManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::signal_replyError)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FtpManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FtpManager.data,
      qt_meta_data_FtpManager,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FtpManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FtpManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FtpManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FtpManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void FtpManager::signal_downloadXmlFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void FtpManager::signal_downloadChFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void FtpManager::signal_downloadEnFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void FtpManager::signal_startDownloadPerFile(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FtpManager::signal_finishDownloadPerFile(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void FtpManager::signal_replyError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
