/****************************************************************************
** Meta object code from reading C++ file 'autoupdaterui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../autoupdaterui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'autoupdaterui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AutoUpdaterUI_t {
    QByteArrayData data[9];
    char stringdata0[163];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AutoUpdaterUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AutoUpdaterUI_t qt_meta_stringdata_AutoUpdaterUI = {
    {
QT_MOC_LITERAL(0, 0, 13), // "AutoUpdaterUI"
QT_MOC_LITERAL(1, 14, 20), // "slotBtnUpdateClicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 21), // "slotBtnRestartClicked"
QT_MOC_LITERAL(4, 58, 17), // "slotUpdateProcess"
QT_MOC_LITERAL(5, 76, 24), // "slotDownloadInitFileOver"
QT_MOC_LITERAL(6, 101, 22), // "slotCheckUpdateTimeOut"
QT_MOC_LITERAL(7, 124, 19), // "slotDownloadTimeout"
QT_MOC_LITERAL(8, 144, 18) // "slotClickTimeoutOk"

    },
    "AutoUpdaterUI\0slotBtnUpdateClicked\0\0"
    "slotBtnRestartClicked\0slotUpdateProcess\0"
    "slotDownloadInitFileOver\0"
    "slotCheckUpdateTimeOut\0slotDownloadTimeout\0"
    "slotClickTimeoutOk"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AutoUpdaterUI[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x09 /* Protected */,
       3,    0,   50,    2, 0x09 /* Protected */,
       4,    0,   51,    2, 0x09 /* Protected */,
       5,    0,   52,    2, 0x09 /* Protected */,
       6,    0,   53,    2, 0x09 /* Protected */,
       7,    0,   54,    2, 0x09 /* Protected */,
       8,    0,   55,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AutoUpdaterUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AutoUpdaterUI *_t = static_cast<AutoUpdaterUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotBtnUpdateClicked(); break;
        case 1: _t->slotBtnRestartClicked(); break;
        case 2: _t->slotUpdateProcess(); break;
        case 3: _t->slotDownloadInitFileOver(); break;
        case 4: _t->slotCheckUpdateTimeOut(); break;
        case 5: _t->slotDownloadTimeout(); break;
        case 6: _t->slotClickTimeoutOk(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject AutoUpdaterUI::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AutoUpdaterUI.data,
      qt_meta_data_AutoUpdaterUI,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AutoUpdaterUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AutoUpdaterUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AutoUpdaterUI.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int AutoUpdaterUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
