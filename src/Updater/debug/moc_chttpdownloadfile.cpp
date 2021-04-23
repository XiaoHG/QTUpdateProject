/****************************************************************************
** Meta object code from reading C++ file 'chttpdownloadfile.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chttpdownloadfile.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chttpdownloadfile.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CHttpDownLoadFile_t {
    QByteArrayData data[19];
    char stringdata0[221];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CHttpDownLoadFile_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CHttpDownLoadFile_t qt_meta_stringdata_CHttpDownLoadFile = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CHttpDownLoadFile"
QT_MOC_LITERAL(1, 18, 19), // "replyNewDataArrived"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 13), // "ReplyFinished"
QT_MOC_LITERAL(4, 53, 12), // "DownLoadFile"
QT_MOC_LITERAL(5, 66, 8), // "XMLParse"
QT_MOC_LITERAL(6, 75, 8), // "filename"
QT_MOC_LITERAL(7, 84, 15), // "downloadXMLFile"
QT_MOC_LITERAL(8, 100, 17), // "getElementVersion"
QT_MOC_LITERAL(9, 118, 3), // "xml"
QT_MOC_LITERAL(10, 122, 4), // "name"
QT_MOC_LITERAL(11, 127, 16), // "CheckUpdateFiles"
QT_MOC_LITERAL(12, 144, 5), // "name1"
QT_MOC_LITERAL(13, 150, 5), // "name2"
QT_MOC_LITERAL(14, 156, 12), // "CheckVersion"
QT_MOC_LITERAL(15, 169, 7), // "version"
QT_MOC_LITERAL(16, 177, 15), // "versionDownload"
QT_MOC_LITERAL(17, 193, 19), // "DownLoadUpdateFiles"
QT_MOC_LITERAL(18, 213, 7) // "ExitApp"

    },
    "CHttpDownLoadFile\0replyNewDataArrived\0"
    "\0ReplyFinished\0DownLoadFile\0XMLParse\0"
    "filename\0downloadXMLFile\0getElementVersion\0"
    "xml\0name\0CheckUpdateFiles\0name1\0name2\0"
    "CheckVersion\0version\0versionDownload\0"
    "DownLoadUpdateFiles\0ExitApp"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CHttpDownLoadFile[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x0a /* Public */,
       3,    0,   65,    2, 0x0a /* Public */,
       4,    0,   66,    2, 0x0a /* Public */,
       5,    1,   67,    2, 0x0a /* Public */,
       7,    0,   70,    2, 0x0a /* Public */,
       8,    2,   71,    2, 0x0a /* Public */,
      11,    2,   76,    2, 0x0a /* Public */,
      14,    2,   81,    2, 0x0a /* Public */,
      17,    0,   86,    2, 0x0a /* Public */,
      18,    1,   87,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::QString, QMetaType::QString, QMetaType::QString,    9,   10,
    QMetaType::Int, QMetaType::QString, QMetaType::QString,   12,   13,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,   15,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,

       0        // eod
};

void CHttpDownLoadFile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CHttpDownLoadFile *_t = static_cast<CHttpDownLoadFile *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->replyNewDataArrived(); break;
        case 1: _t->ReplyFinished(); break;
        case 2: _t->DownLoadFile(); break;
        case 3: _t->XMLParse((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->downloadXMLFile(); break;
        case 5: { QString _r = _t->getElementVersion((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 6: { int _r = _t->CheckUpdateFiles((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->CheckVersion((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->DownLoadUpdateFiles(); break;
        case 9: _t->ExitApp((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CHttpDownLoadFile::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CHttpDownLoadFile.data,
      qt_meta_data_CHttpDownLoadFile,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CHttpDownLoadFile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CHttpDownLoadFile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CHttpDownLoadFile.stringdata0))
        return static_cast<void*>(const_cast< CHttpDownLoadFile*>(this));
    return QWidget::qt_metacast(_clname);
}

int CHttpDownLoadFile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
