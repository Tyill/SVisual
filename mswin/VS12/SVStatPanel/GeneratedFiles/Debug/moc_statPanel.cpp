/****************************************************************************
** Meta object code from reading C++ file 'statPanel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/SVStatPanel/forms/statPanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'statPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_statPanel_t {
    QByteArrayData data[13];
    char stringdata0[158];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_statPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_statPanel_t qt_meta_stringdata_statPanel = {
    {
QT_MOC_LITERAL(0, 0, 9), // "statPanel"
QT_MOC_LITERAL(1, 10, 14), // "dragEnterEvent"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 16), // "QDragEnterEvent*"
QT_MOC_LITERAL(4, 43, 5), // "event"
QT_MOC_LITERAL(5, 49, 13), // "dragMoveEvent"
QT_MOC_LITERAL(6, 63, 15), // "QDragMoveEvent*"
QT_MOC_LITERAL(7, 79, 9), // "dropEvent"
QT_MOC_LITERAL(8, 89, 11), // "QDropEvent*"
QT_MOC_LITERAL(9, 101, 16), // "graphValueChange"
QT_MOC_LITERAL(10, 118, 18), // "selectSignalChange"
QT_MOC_LITERAL(11, 137, 16), // "selectSignalTime"
QT_MOC_LITERAL(12, 154, 3) // "row"

    },
    "statPanel\0dragEnterEvent\0\0QDragEnterEvent*\0"
    "event\0dragMoveEvent\0QDragMoveEvent*\0"
    "dropEvent\0QDropEvent*\0graphValueChange\0"
    "selectSignalChange\0selectSignalTime\0"
    "row"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_statPanel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x08 /* Private */,
       5,    1,   47,    2, 0x08 /* Private */,
       7,    1,   50,    2, 0x08 /* Private */,
       9,    0,   53,    2, 0x08 /* Private */,
      10,    0,   54,    2, 0x08 /* Private */,
      11,    1,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void, 0x80000000 | 8,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,

       0        // eod
};

void statPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        statPanel *_t = static_cast<statPanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dragEnterEvent((*reinterpret_cast< QDragEnterEvent*(*)>(_a[1]))); break;
        case 1: _t->dragMoveEvent((*reinterpret_cast< QDragMoveEvent*(*)>(_a[1]))); break;
        case 2: _t->dropEvent((*reinterpret_cast< QDropEvent*(*)>(_a[1]))); break;
        case 3: _t->graphValueChange(); break;
        case 4: _t->selectSignalChange(); break;
        case 5: _t->selectSignalTime((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject statPanel::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_statPanel.data,
      qt_meta_data_statPanel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *statPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *statPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_statPanel.stringdata0))
        return static_cast<void*>(const_cast< statPanel*>(this));
    return QDialog::qt_metacast(_clname);
}

int statPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
