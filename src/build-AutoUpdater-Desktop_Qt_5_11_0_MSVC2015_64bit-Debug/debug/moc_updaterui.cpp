/****************************************************************************
** Meta object code from reading C++ file 'updaterui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AutoUpdater/updaterui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'updaterui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UpdaterUI_t {
    QByteArrayData data[14];
    char stringdata0[261];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UpdaterUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UpdaterUI_t qt_meta_stringdata_UpdaterUI = {
    {
QT_MOC_LITERAL(0, 0, 9), // "UpdaterUI"
QT_MOC_LITERAL(1, 10, 21), // "on_btn_update_clicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 22), // "on_btn_restart_clicked"
QT_MOC_LITERAL(4, 56, 17), // "on_timer_updating"
QT_MOC_LITERAL(5, 74, 33), // "on_updater_initFileDownloadFi..."
QT_MOC_LITERAL(6, 108, 4), // "name"
QT_MOC_LITERAL(7, 113, 23), // "on_timer_checkForUpdate"
QT_MOC_LITERAL(8, 137, 19), // "on_btn_exit_clicked"
QT_MOC_LITERAL(9, 157, 31), // "on_updater_startDownloadPerFile"
QT_MOC_LITERAL(10, 189, 8), // "fileName"
QT_MOC_LITERAL(11, 198, 32), // "on_updater_finishDownloadPerFile"
QT_MOC_LITERAL(12, 231, 22), // "on_updater_reportError"
QT_MOC_LITERAL(13, 254, 6) // "errStr"

    },
    "UpdaterUI\0on_btn_update_clicked\0\0"
    "on_btn_restart_clicked\0on_timer_updating\0"
    "on_updater_initFileDownloadFinish\0"
    "name\0on_timer_checkForUpdate\0"
    "on_btn_exit_clicked\0on_updater_startDownloadPerFile\0"
    "fileName\0on_updater_finishDownloadPerFile\0"
    "on_updater_reportError\0errStr"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UpdaterUI[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x09 /* Protected */,
       3,    0,   60,    2, 0x09 /* Protected */,
       4,    0,   61,    2, 0x09 /* Protected */,
       5,    1,   62,    2, 0x09 /* Protected */,
       7,    0,   65,    2, 0x09 /* Protected */,
       8,    0,   66,    2, 0x09 /* Protected */,
       9,    1,   67,    2, 0x09 /* Protected */,
      11,    1,   70,    2, 0x09 /* Protected */,
      12,    1,   73,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   13,

       0        // eod
};

void UpdaterUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UpdaterUI *_t = static_cast<UpdaterUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btn_update_clicked(); break;
        case 1: _t->on_btn_restart_clicked(); break;
        case 2: _t->on_timer_updating(); break;
        case 3: _t->on_updater_initFileDownloadFinish((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->on_timer_checkForUpdate(); break;
        case 5: _t->on_btn_exit_clicked(); break;
        case 6: _t->on_updater_startDownloadPerFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->on_updater_finishDownloadPerFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->on_updater_reportError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UpdaterUI::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_UpdaterUI.data,
      qt_meta_data_UpdaterUI,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *UpdaterUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UpdaterUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UpdaterUI.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int UpdaterUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
