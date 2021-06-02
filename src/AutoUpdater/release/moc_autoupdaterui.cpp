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
    QByteArrayData data[11];
    char stringdata0[230];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AutoUpdaterUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AutoUpdaterUI_t qt_meta_stringdata_AutoUpdaterUI = {
    {
QT_MOC_LITERAL(0, 0, 13), // "AutoUpdaterUI"
QT_MOC_LITERAL(1, 14, 21), // "on_btn_update_clicked"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 22), // "on_btn_restart_clicked"
QT_MOC_LITERAL(4, 60, 17), // "on_timer_updating"
QT_MOC_LITERAL(5, 78, 33), // "on_updater_initFileDownloadFi..."
QT_MOC_LITERAL(6, 112, 23), // "on_timer_checkForUpdate"
QT_MOC_LITERAL(7, 136, 19), // "on_btn_exit_clicked"
QT_MOC_LITERAL(8, 156, 31), // "on_updater_startDownloadPerFile"
QT_MOC_LITERAL(9, 188, 8), // "fileName"
QT_MOC_LITERAL(10, 197, 32) // "on_updater_finishDownloadPerFile"

    },
    "AutoUpdaterUI\0on_btn_update_clicked\0"
    "\0on_btn_restart_clicked\0on_timer_updating\0"
    "on_updater_initFileDownloadFinish\0"
    "on_timer_checkForUpdate\0on_btn_exit_clicked\0"
    "on_updater_startDownloadPerFile\0"
    "fileName\0on_updater_finishDownloadPerFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AutoUpdaterUI[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x09 /* Protected */,
       3,    0,   55,    2, 0x09 /* Protected */,
       4,    0,   56,    2, 0x09 /* Protected */,
       5,    0,   57,    2, 0x09 /* Protected */,
       6,    0,   58,    2, 0x09 /* Protected */,
       7,    0,   59,    2, 0x09 /* Protected */,
       8,    1,   60,    2, 0x09 /* Protected */,
      10,    1,   63,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,

       0        // eod
};

void AutoUpdaterUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AutoUpdaterUI *_t = static_cast<AutoUpdaterUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btn_update_clicked(); break;
        case 1: _t->on_btn_restart_clicked(); break;
        case 2: _t->on_timer_updating(); break;
        case 3: _t->on_updater_initFileDownloadFinish(); break;
        case 4: _t->on_timer_checkForUpdate(); break;
        case 5: _t->on_btn_exit_clicked(); break;
        case 6: _t->on_updater_startDownloadPerFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->on_updater_finishDownloadPerFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AutoUpdaterUI::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_AutoUpdaterUI.data,
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
    return QMainWindow::qt_metacast(_clname);
}

int AutoUpdaterUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
