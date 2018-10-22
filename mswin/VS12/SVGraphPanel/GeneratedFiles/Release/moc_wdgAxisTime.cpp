/****************************************************************************
** Meta object code from reading C++ file 'wdgAxisTime.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/SVGraphPanel/src/wdgAxisTime.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wdgAxisTime.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_wdgAxisTime_t {
    QByteArrayData data[3];
    char stringdata0[28];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_wdgAxisTime_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_wdgAxisTime_t qt_meta_stringdata_wdgAxisTime = {
    {
QT_MOC_LITERAL(0, 0, 11), // "wdgAxisTime"
QT_MOC_LITERAL(1, 12, 14), // "req_axisChange"
QT_MOC_LITERAL(2, 27, 0) // ""

    },
    "wdgAxisTime\0req_axisChange\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_wdgAxisTime[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,

       0        // eod
};

void wdgAxisTime::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        wdgAxisTime *_t = static_cast<wdgAxisTime *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->req_axisChange(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (wdgAxisTime::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&wdgAxisTime::req_axisChange)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject wdgAxisTime::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_wdgAxisTime.data,
      qt_meta_data_wdgAxisTime,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *wdgAxisTime::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *wdgAxisTime::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_wdgAxisTime.stringdata0))
        return static_cast<void*>(const_cast< wdgAxisTime*>(this));
    return QWidget::qt_metacast(_clname);
}

int wdgAxisTime::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void wdgAxisTime::req_axisChange()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
