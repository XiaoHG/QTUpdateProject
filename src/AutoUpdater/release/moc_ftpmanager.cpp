/****************************************************************************
** Meta object code from reading C++ file 'ftpmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ftpmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ftpmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FtpManager_t {
    QByteArrayData data[11];
    char stringdata0[200];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FtpManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FtpManager_t qt_meta_stringdata_FtpManager = {
    {
QT_MOC_LITERAL(0, 0, 10), // "FtpManager"
QT_MOC_LITERAL(1, 11, 28), // "signal_downloadLatestVFinish"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 20), // "signal_startDownload"
QT_MOC_LITERAL(4, 62, 21), // "signal_finishDownload"
QT_MOC_LITERAL(5, 84, 17), // "signal_replyError"
QT_MOC_LITERAL(6, 102, 23), // "on_reply_downloadFinish"
QT_MOC_LITERAL(7, 126, 20), // "on_timer_downloading"
QT_MOC_LITERAL(8, 147, 14), // "on_reply_error"
QT_MOC_LITERAL(9, 162, 27), // "QNetworkReply::NetworkError"
QT_MOC_LITERAL(10, 190, 9) // "errorCode"

    },
    "FtpManager\0signal_downloadLatestVFinish\0"
    "\0signal_startDownload\0signal_finishDownload\0"
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
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    1,   50,    2, 0x06 /* Public */,
       4,    1,   53,    2, 0x06 /* Public */,
       5,    1,   56,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   59,    2, 0x09 /* Protected */,
       7,    0,   60,    2, 0x09 /* Protected */,
       8,    1,   61,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void FtpManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FtpManager *_t = static_cast<FtpManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signal_downloadLatestVFinish(); break;
        case 1: _t->signal_startDownload((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->signal_finishDownload((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->signal_replyError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->on_reply_downloadFinish(); break;
        case 5: _t->on_timer_downloading(); break;
        case 6: _t->on_reply_error((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (FtpManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FtpManager::signal_downloadLatestVFinish)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (FtpManager::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FtpManager::signal_startDownload)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (FtpManager::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FtpManager::signal_finishDownload)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (FtpManager::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&FtpManager::signal_replyError)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject FtpManager::staticMetaObject = {
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
        return static_cast<void*>(const_cast< FtpManager*>(this));
    return QObject::qt_metacast(_clname);
}

int FtpManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void FtpManager::signal_downloadLatestVFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void FtpManager::signal_startDownload(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FtpManager::signal_finishDownload(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FtpManager::signal_replyError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
