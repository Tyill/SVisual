/****************************************************************************
** Meta object code from reading C++ file 'settingsPanel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/SVMonitor/forms/settingsPanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settingsPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_settingsPanel_t {
    QByteArrayData data[10];
    char stringdata0[78];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_settingsPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_settingsPanel_t qt_meta_stringdata_settingsPanel = {
    {
QT_MOC_LITERAL(0, 0, 13), // "settingsPanel"
QT_MOC_LITERAL(1, 14, 10), // "saveChange"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 6), // "addCOM"
QT_MOC_LITERAL(4, 33, 4), // "port"
QT_MOC_LITERAL(5, 38, 5), // "speed"
QT_MOC_LITERAL(6, 44, 3), // "row"
QT_MOC_LITERAL(7, 48, 6), // "delCOM"
QT_MOC_LITERAL(8, 55, 10), // "selDirArch"
QT_MOC_LITERAL(9, 66, 11) // "paramChange"

    },
    "settingsPanel\0saveChange\0\0addCOM\0port\0"
    "speed\0row\0delCOM\0selDirArch\0paramChange"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_settingsPanel[] = {

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
       1,    0,   54,    2, 0x08 /* Private */,
       3,    3,   55,    2, 0x08 /* Private */,
       3,    2,   62,    2, 0x28 /* Private | MethodCloned */,
       3,    1,   67,    2, 0x28 /* Private | MethodCloned */,
       3,    0,   70,    2, 0x28 /* Private | MethodCloned */,
       7,    0,   71,    2, 0x08 /* Private */,
       8,    0,   72,    2, 0x08 /* Private */,
       9,    0,   73,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,    4,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    4,    5,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void settingsPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        settingsPanel *_t = static_cast<settingsPanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->saveChange(); break;
        case 1: _t->addCOM((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->addCOM((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->addCOM((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->addCOM(); break;
        case 5: _t->delCOM(); break;
        case 6: _t->selDirArch(); break;
        case 7: _t->paramChange(); break;
        default: ;
        }
    }
}

const QMetaObject settingsPanel::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_settingsPanel.data,
      qt_meta_data_settingsPanel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *settingsPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *settingsPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_settingsPanel.stringdata0))
        return static_cast<void*>(const_cast< settingsPanel*>(this));
    return QDialog::qt_metacast(_clname);
}

int settingsPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
