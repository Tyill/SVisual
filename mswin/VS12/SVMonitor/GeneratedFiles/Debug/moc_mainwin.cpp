/****************************************************************************
** Meta object code from reading C++ file 'mainwin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/SVMonitor/forms/mainwin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWin_t {
    QByteArrayData data[20];
    char stringdata0[225];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWin_t qt_meta_stringdata_MainWin = {
    {
QT_MOC_LITERAL(0, 0, 7), // "MainWin"
QT_MOC_LITERAL(1, 8, 8), // "slowMode"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 14), // "selSignalClick"
QT_MOC_LITERAL(4, 33, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(5, 50, 15), // "selSignalDClick"
QT_MOC_LITERAL(6, 66, 4), // "item"
QT_MOC_LITERAL(7, 71, 6), // "column"
QT_MOC_LITERAL(8, 78, 15), // "selSignalChange"
QT_MOC_LITERAL(9, 94, 16), // "contextMenuClick"
QT_MOC_LITERAL(10, 111, 8), // "QAction*"
QT_MOC_LITERAL(11, 120, 13), // "StatusTxtMess"
QT_MOC_LITERAL(12, 134, 4), // "mess"
QT_MOC_LITERAL(13, 139, 15), // "updateTblSignal"
QT_MOC_LITERAL(14, 155, 13), // "updateSignals"
QT_MOC_LITERAL(15, 169, 13), // "moduleConnect"
QT_MOC_LITERAL(16, 183, 6), // "module"
QT_MOC_LITERAL(17, 190, 16), // "moduleDisconnect"
QT_MOC_LITERAL(18, 207, 9), // "onTrigger"
QT_MOC_LITERAL(19, 217, 7) // "trigger"

    },
    "MainWin\0slowMode\0\0selSignalClick\0"
    "QTreeWidgetItem*\0selSignalDClick\0item\0"
    "column\0selSignalChange\0contextMenuClick\0"
    "QAction*\0StatusTxtMess\0mess\0updateTblSignal\0"
    "updateSignals\0moduleConnect\0module\0"
    "moduleDisconnect\0onTrigger\0trigger"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x0a /* Public */,
       3,    2,   70,    2, 0x0a /* Public */,
       5,    2,   75,    2, 0x0a /* Public */,
       8,    2,   80,    2, 0x0a /* Public */,
       9,    1,   85,    2, 0x0a /* Public */,
      11,    1,   88,    2, 0x0a /* Public */,
      13,    0,   91,    2, 0x0a /* Public */,
      14,    0,   92,    2, 0x0a /* Public */,
      15,    1,   93,    2, 0x0a /* Public */,
      17,    1,   96,    2, 0x0a /* Public */,
      18,    1,   99,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    2,    2,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    6,    7,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    6,    7,
    QMetaType::Void, 0x80000000 | 10,    2,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QString,   19,

       0        // eod
};

void MainWin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWin *_t = static_cast<MainWin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slowMode(); break;
        case 1: _t->selSignalClick((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->selSignalDClick((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->selSignalChange((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->contextMenuClick((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 5: _t->StatusTxtMess((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->updateTblSignal(); break;
        case 7: _t->updateSignals(); break;
        case 8: _t->moduleConnect((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->moduleDisconnect((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->onTrigger((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MainWin::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWin.data,
      qt_meta_data_MainWin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWin.stringdata0))
        return static_cast<void*>(const_cast< MainWin*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
