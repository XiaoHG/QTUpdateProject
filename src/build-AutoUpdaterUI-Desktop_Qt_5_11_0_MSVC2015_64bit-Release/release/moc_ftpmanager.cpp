/****************************************************************************
** Meta object code from reading C++ file 'ftpmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "C:/Users/anycubic/Desktop/0514/QTUpdateProject-home/src/AutoUpdaterUI/ftpmanager.h"
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
    QByteArrayData data[10];
    char stringdata0[184];
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
QT_MOC_LITERAL(4, 69, 27), // "sigSingleFileDownloadFinish"
QT_MOC_LITERAL(5, 97, 20), // "sigAllFileDownFinish"
QT_MOC_LITERAL(6, 118, 14), // "uploadFinished"
QT_MOC_LITERAL(7, 133, 16), // "downloadFinished"
QT_MOC_LITERAL(8, 150, 5), // "error"
QT_MOC_LITERAL(9, 156, 27) // "QNetworkReply::NetworkError"

    },
    "FtpManager\0sigDownloadUpdaterXmlOver\0"
    "\0sigDownloadVersionInfoFileOver\0"
    "sigSingleFileDownloadFinish\0"
    "sigAllFileDownFinish\0uploadFinished\0"
    "downloadFinished\0error\0"
    "QNetworkReply::NetworkError"
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
       3,    0,   50,    2, 0x06 /* Public */,
       4,    1,   51,    2, 0x06 /* Public */,
       5,    0,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   55,    2, 0x0a /* Public */,
       7,    0,   56,    2, 0x0a /* Public */,
       8,    1,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,    2,

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
        case 2: _t->sigSingleFileDownloadFinish((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->sigAllFileDownFinish(); break;
        case 4: _t->uploadFinished(); break;
        case 5: _t->downloadFinished(); break;
        case 6: _t->error((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
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
            using _t = void (FtpManager::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::sigSingleFileDownloadFinish)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (FtpManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpManager::sigAllFileDownFinish)) {
                *result = 3;
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
void FtpManager::sigSingleFileDownloadFinish(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FtpManager::sigAllFileDownFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
