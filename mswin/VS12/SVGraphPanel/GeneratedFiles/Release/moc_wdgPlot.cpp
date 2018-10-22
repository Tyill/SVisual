/****************************************************************************
** Meta object code from reading C++ file 'wdgPlot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/SVGraphPanel/src/wdgPlot.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wdgPlot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_wdgPlot_t {
    QByteArrayData data[5];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_wdgPlot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_wdgPlot_t qt_meta_stringdata_wdgPlot = {
    {
QT_MOC_LITERAL(0, 0, 7), // "wdgPlot"
QT_MOC_LITERAL(1, 8, 13), // "req_rctChange"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 14), // "req_moveChange"
QT_MOC_LITERAL(4, 38, 13) // "req_updMarker"

    },
    "wdgPlot\0req_rctChange\0\0req_moveChange\0"
    "req_updMarker"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_wdgPlot[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    0,   30,    2, 0x06 /* Public */,
       4,    0,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void wdgPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        wdgPlot *_t = static_cast<wdgPlot *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->req_rctChange(); break;
        case 1: _t->req_moveChange(); break;
        case 2: _t->req_updMarker(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (wdgPlot::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&wdgPlot::req_rctChange)) {
                *result = 0;
            }
        }
        {
            typedef void (wdgPlot::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&wdgPlot::req_moveChange)) {
                *result = 1;
            }
        }
        {
            typedef void (wdgPlot::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&wdgPlot::req_updMarker)) {
                *result = 2;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject wdgPlot::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_wdgPlot.data,
      qt_meta_data_wdgPlot,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *wdgPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *wdgPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_wdgPlot.stringdata0))
        return static_cast<void*>(const_cast< wdgPlot*>(this));
    return QWidget::qt_metacast(_clname);
}

int wdgPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void wdgPlot::req_rctChange()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void wdgPlot::req_moveChange()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void wdgPlot::req_updMarker()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
