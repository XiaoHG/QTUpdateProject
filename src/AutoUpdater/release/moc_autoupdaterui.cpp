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
#include <QtCore/QList>
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
    QByteArrayData data[22];
    char stringdata0[342];
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
QT_MOC_LITERAL(4, 58, 20), // "slotUpdatingTimerout"
QT_MOC_LITERAL(5, 79, 24), // "slotDownloadInitFileOver"
QT_MOC_LITERAL(6, 104, 25), // "slotCheckForUpdateTimeout"
QT_MOC_LITERAL(7, 130, 15), // "slotClickOkExit"
QT_MOC_LITERAL(8, 146, 24), // "slotDownloadStartPerFile"
QT_MOC_LITERAL(9, 171, 8), // "fileName"
QT_MOC_LITERAL(10, 180, 25), // "slotDownloadFinishPerFile"
QT_MOC_LITERAL(11, 206, 11), // "ShowWhichUI"
QT_MOC_LITERAL(12, 218, 15), // "QList<QWidget*>"
QT_MOC_LITERAL(13, 234, 7), // "widgets"
QT_MOC_LITERAL(14, 242, 7), // "visible"
QT_MOC_LITERAL(15, 250, 15), // "mousePressEvent"
QT_MOC_LITERAL(16, 266, 12), // "QMouseEvent*"
QT_MOC_LITERAL(17, 279, 5), // "event"
QT_MOC_LITERAL(18, 285, 14), // "mouseMoveEvent"
QT_MOC_LITERAL(19, 300, 17), // "mouseReleaseEvent"
QT_MOC_LITERAL(20, 318, 10), // "paintEvent"
QT_MOC_LITERAL(21, 329, 12) // "QPaintEvent*"

    },
    "AutoUpdaterUI\0slotBtnUpdateClicked\0\0"
    "slotBtnRestartClicked\0slotUpdatingTimerout\0"
    "slotDownloadInitFileOver\0"
    "slotCheckForUpdateTimeout\0slotClickOkExit\0"
    "slotDownloadStartPerFile\0fileName\0"
    "slotDownloadFinishPerFile\0ShowWhichUI\0"
    "QList<QWidget*>\0widgets\0visible\0"
    "mousePressEvent\0QMouseEvent*\0event\0"
    "mouseMoveEvent\0mouseReleaseEvent\0"
    "paintEvent\0QPaintEvent*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AutoUpdaterUI[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x09 /* Protected */,
       3,    0,   80,    2, 0x09 /* Protected */,
       4,    0,   81,    2, 0x09 /* Protected */,
       5,    0,   82,    2, 0x09 /* Protected */,
       6,    0,   83,    2, 0x09 /* Protected */,
       7,    0,   84,    2, 0x09 /* Protected */,
       8,    1,   85,    2, 0x09 /* Protected */,
      10,    1,   88,    2, 0x09 /* Protected */,
      11,    2,   91,    2, 0x09 /* Protected */,
      15,    1,   96,    2, 0x09 /* Protected */,
      18,    1,   99,    2, 0x09 /* Protected */,
      19,    1,  102,    2, 0x09 /* Protected */,
      20,    1,  105,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, 0x80000000 | 12, QMetaType::Bool,   13,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 21,   17,

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
        case 2: _t->slotUpdatingTimerout(); break;
        case 3: _t->slotDownloadInitFileOver(); break;
        case 4: _t->slotCheckForUpdateTimeout(); break;
        case 5: _t->slotClickOkExit(); break;
        case 6: _t->slotDownloadStartPerFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->slotDownloadFinishPerFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->ShowWhichUI((*reinterpret_cast< const QList<QWidget*>(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 9: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 10: _t->mouseMoveEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 11: _t->mouseReleaseEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 12: _t->paintEvent((*reinterpret_cast< QPaintEvent*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QWidget*> >(); break;
            }
            break;
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
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
