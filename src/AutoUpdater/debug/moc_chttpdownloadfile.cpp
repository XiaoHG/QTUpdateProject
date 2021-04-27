/****************************************************************************
** Meta object code from reading C++ file 'chttpdownloadfile.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chttpdownloadfile.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chttpdownloadfile.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CHttpDownloadFile_t {
    QByteArrayData data[10];
    char stringdata0[181];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CHttpDownloadFile_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CHttpDownloadFile_t qt_meta_stringdata_CHttpDownloadFile = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CHttpDownloadFile"
QT_MOC_LITERAL(1, 18, 25), // "sigDownloadFinishedSignal"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 23), // "slotReplyNewDataArrived"
QT_MOC_LITERAL(4, 69, 17), // "slotReplyFinished"
QT_MOC_LITERAL(5, 87, 14), // "slotReplyError"
QT_MOC_LITERAL(6, 102, 27), // "QNetworkReply::NetworkError"
QT_MOC_LITERAL(7, 130, 25), // "slotReplyDownloadProgress"
QT_MOC_LITERAL(8, 156, 13), // "bytesReceived"
QT_MOC_LITERAL(9, 170, 10) // "bytesTotal"

    },
    "CHttpDownloadFile\0sigDownloadFinishedSignal\0"
    "\0slotReplyNewDataArrived\0slotReplyFinished\0"
    "slotReplyError\0QNetworkReply::NetworkError\0"
    "slotReplyDownloadProgress\0bytesReceived\0"
    "bytesTotal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CHttpDownloadFile[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   40,    2, 0x0a /* Public */,
       4,    0,   41,    2, 0x0a /* Public */,
       5,    1,   42,    2, 0x0a /* Public */,
       7,    2,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    8,    9,

       0        // eod
};

void CHttpDownloadFile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CHttpDownloadFile *_t = static_cast<CHttpDownloadFile *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigDownloadFinishedSignal(); break;
        case 1: _t->slotReplyNewDataArrived(); break;
        case 2: _t->slotReplyFinished(); break;
        case 3: _t->slotReplyError((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 4: _t->slotReplyDownloadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
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
            using _t = void (CHttpDownloadFile::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CHttpDownloadFile::sigDownloadFinishedSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CHttpDownloadFile::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CHttpDownloadFile.data,
      qt_meta_data_CHttpDownloadFile,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CHttpDownloadFile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CHttpDownloadFile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CHttpDownloadFile.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CHttpDownloadFile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CHttpDownloadFile::sigDownloadFinishedSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
