/****************************************************************************
** Meta object code from reading C++ file 'triggerPanel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/SVMonitor/forms/triggerPanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'triggerPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_triggerPanel_t {
    QByteArrayData data[15];
    char stringdata0[175];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_triggerPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_triggerPanel_t qt_meta_stringdata_triggerPanel = {
    {
QT_MOC_LITERAL(0, 0, 12), // "triggerPanel"
QT_MOC_LITERAL(1, 13, 9), // "selModule"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(4, 41, 4), // "item"
QT_MOC_LITERAL(5, 46, 9), // "selSignal"
QT_MOC_LITERAL(6, 56, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(7, 74, 10), // "selTrigger"
QT_MOC_LITERAL(8, 85, 10), // "addTrigger"
QT_MOC_LITERAL(9, 96, 10), // "delTrigger"
QT_MOC_LITERAL(10, 107, 13), // "changeTrigger"
QT_MOC_LITERAL(11, 121, 11), // "paramChange"
QT_MOC_LITERAL(12, 133, 12), // "selCondition"
QT_MOC_LITERAL(13, 146, 17), // "SV_Cng::eventType"
QT_MOC_LITERAL(14, 164, 10) // "selDirProc"

    },
    "triggerPanel\0selModule\0\0QListWidgetItem*\0"
    "item\0selSignal\0QTableWidgetItem*\0"
    "selTrigger\0addTrigger\0delTrigger\0"
    "changeTrigger\0paramChange\0selCondition\0"
    "SV_Cng::eventType\0selDirProc"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_triggerPanel[] = {

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
       1,    1,   59,    2, 0x08 /* Private */,
       5,    1,   62,    2, 0x08 /* Private */,
       7,    1,   65,    2, 0x08 /* Private */,
       8,    0,   68,    2, 0x08 /* Private */,
       9,    0,   69,    2, 0x08 /* Private */,
      10,    0,   70,    2, 0x08 /* Private */,
      11,    0,   71,    2, 0x08 /* Private */,
      12,    1,   72,    2, 0x08 /* Private */,
      14,    0,   75,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,    2,
    QMetaType::Void,

       0        // eod
};

void triggerPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        triggerPanel *_t = static_cast<triggerPanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selModule((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->selSignal((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->selTrigger((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->addTrigger(); break;
        case 4: _t->delTrigger(); break;
        case 5: _t->changeTrigger(); break;
        case 6: _t->paramChange(); break;
        case 7: _t->selCondition((*reinterpret_cast< SV_Cng::eventType(*)>(_a[1]))); break;
        case 8: _t->selDirProc(); break;
        default: ;
        }
    }
}

const QMetaObject triggerPanel::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_triggerPanel.data,
      qt_meta_data_triggerPanel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *triggerPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *triggerPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_triggerPanel.stringdata0))
        return static_cast<void*>(const_cast< triggerPanel*>(this));
    return QDialog::qt_metacast(_clname);
}

int triggerPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
