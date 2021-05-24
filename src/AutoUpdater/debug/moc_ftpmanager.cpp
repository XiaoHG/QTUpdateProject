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
    QByteArrayData data[14];
    char stringdata0[276];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FtpManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FtpManager_t qt_meta_stringdata_FtpManager = {
    {
QT_MOC_LITERAL(0, 0, 10), // "FtpManager"
QT_MOC_LITERAL(1, 11, 25), // "sigDownloadUpdaterXmlOver"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 30), // "sigDownloadVersionInfoFileOver"
QT_MOC_LITERAL(4, 69, 32), // "sigDownloadVersionInfoEnfileOver"
QT_MOC_LITERAL(5, 102, 23), // "sigDownloadStartPerFile"
QT_MOC_LITERAL(6, 126, 24), // "sigDownloadFinishPerFile"
QT_MOC_LITERAL(7, 151, 20), // "sigAllFileDownFinish"
QT_MOC_LITERAL(8, 172, 18), // "sigDownloadTimeout"
QT_MOC_LITERAL(9, 191, 13), // "sigReplyError"
QT_MOC_LITERAL(10, 205, 16), // "downloadFinished"
QT_MOC_LITERAL(11, 222, 19), // "slotDownloadTimeout"
QT_MOC_LITERAL(12, 242, 5), // "error"
QT_MOC_LITERAL(13, 248, 27) // "QNetworkReply::NetworkError"

    },
    "FtpManager\0sigDownloadUpdaterXmlOver\0"
    "\0sigDownloadVersionInfoFileOver\0"
    "sigDownloadVersionInfoEnfileOver\0"
    "sigDownloadStartPerFile\0"
    "sigDownloadFinishPerFile\0sigAllFileDownFinish\0"
    "sigDownloadTimeout\0sigReplyError\0"
    "downloadFinished\0slotDownloadTimeout\0"
    "error\0QNetworkReply::NetworkError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FtpManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    0,   70,    2, 0x06 /* Public */,
       4,    0,   71,    2, 0x06 /* Public */,
       5,    1,   72,    2, 0x06 /* Public */,
       6,    1,   75,    2, 0x06 /* Public */,
       7,    0,   78,    2, 0x06 /* Public */,
       8,    1,   79,    2, 0x06 /* Public */,
       9,    1,   82,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   85,    2, 0x09 /* Protected */,
      11,    0,   86,    2, 0x09 /* Protected */,
      12,    1,   87,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,    2,

       0        // eod
};

void FtpManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FtpManager *_t = static_cast<FtpManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigDownloadUpdaterXmlOver(); break;
        case 1: _t->sigDownloadVersionInfoFileOver(); break;
        case 2: _t->sigDownloadVersionInfoEnfileOver(); break;
        case 3: _t->sigDownloadStartPerFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->sigDownloadFinishPerFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->sigAllFileDownFinish(); break;
        case 6: _t->sigDownloadTimeout((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->sigReplyError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->downloadFinished(); break;
        case 9: _t->slotDownloadTimeout(); break;
        case 10: _t->error((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 10:
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
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::sigDownloadUpdaterXmlOver)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FtpManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::sigDownloadVersionInfoFileOver)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FtpManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::sigDownloadVersionInfoEnfileOver)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (FtpManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::sigDownloadStartPerFile)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (FtpManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::sigDownloadFinishPerFile)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (FtpManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::sigAllFileDownFinish)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (FtpManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::sigDownloadTimeout)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (FtpManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::sigReplyError)) {
                *result = 7;
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
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void FtpManager::sigDownloadUpdaterXmlOver()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void FtpManager::sigDownloadVersionInfoFileOver()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void FtpManager::sigDownloadVersionInfoEnfileOver()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void FtpManager::sigDownloadStartPerFile(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FtpManager::sigDownloadFinishPerFile(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void FtpManager::sigAllFileDownFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void FtpManager::sigDownloadTimeout(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void FtpManager::sigReplyError(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
