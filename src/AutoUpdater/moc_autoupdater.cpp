/****************************************************************************
** Meta object code from reading C++ file 'autoupdater.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "autoupdater.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'autoupdater.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AutoUpdater_t {
    QByteArrayData data[13];
    char stringdata0[275];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AutoUpdater_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AutoUpdater_t qt_meta_stringdata_AutoUpdater = {
    {
QT_MOC_LITERAL(0, 0, 11), // "AutoUpdater"
QT_MOC_LITERAL(1, 12, 29), // "signal_initFileDownloadFinish"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 27), // "signal_startDownloadPerFile"
QT_MOC_LITERAL(4, 71, 28), // "signal_finishDownloadPerFile"
QT_MOC_LITERAL(5, 100, 24), // "on_ftp_downloadXmlFinish"
QT_MOC_LITERAL(6, 125, 23), // "on_ftp_downloadChFinish"
QT_MOC_LITERAL(7, 149, 23), // "on_ftp_downloadEnFinish"
QT_MOC_LITERAL(8, 173, 28), // "on_ftp_storageDownloadErrStr"
QT_MOC_LITERAL(9, 202, 6), // "errStr"
QT_MOC_LITERAL(10, 209, 27), // "on_ftp_startDownloadPerFile"
QT_MOC_LITERAL(11, 237, 8), // "fileName"
QT_MOC_LITERAL(12, 246, 28) // "on_ftp_finishDownloadPerFile"

    },
    "AutoUpdater\0signal_initFileDownloadFinish\0"
    "\0signal_startDownloadPerFile\0"
    "signal_finishDownloadPerFile\0"
    "on_ftp_downloadXmlFinish\0"
    "on_ftp_downloadChFinish\0on_ftp_downloadEnFinish\0"
    "on_ftp_storageDownloadErrStr\0errStr\0"
    "on_ftp_startDownloadPerFile\0fileName\0"
    "on_ftp_finishDownloadPerFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AutoUpdater[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    1,   60,    2, 0x06 /* Public */,
       4,    1,   63,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   66,    2, 0x09 /* Protected */,
       6,    0,   67,    2, 0x09 /* Protected */,
       7,    0,   68,    2, 0x09 /* Protected */,
       8,    1,   69,    2, 0x09 /* Protected */,
      10,    1,   72,    2, 0x09 /* Protected */,
      12,    1,   75,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString,   11,

       0        // eod
};

void AutoUpdater::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AutoUpdater *_t = static_cast<AutoUpdater *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signal_initFileDownloadFinish(); break;
        case 1: _t->signal_startDownloadPerFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->signal_finishDownloadPerFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->on_ftp_downloadXmlFinish(); break;
        case 4: _t->on_ftp_downloadChFinish(); break;
        case 5: _t->on_ftp_downloadEnFinish(); break;
        case 6: _t->on_ftp_storageDownloadErrStr((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->on_ftp_startDownloadPerFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->on_ftp_finishDownloadPerFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AutoUpdater::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AutoUpdater::signal_initFileDownloadFinish)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AutoUpdater::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AutoUpdater::signal_startDownloadPerFile)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AutoUpdater::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AutoUpdater::signal_finishDownloadPerFile)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject AutoUpdater::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AutoUpdater.data,
      qt_meta_data_AutoUpdater,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AutoUpdater::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AutoUpdater::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AutoUpdater.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AutoUpdater::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void AutoUpdater::signal_initFileDownloadFinish()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AutoUpdater::signal_startDownloadPerFile(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AutoUpdater::signal_finishDownloadPerFile(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
