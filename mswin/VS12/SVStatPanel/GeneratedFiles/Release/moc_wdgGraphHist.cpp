/****************************************************************************
** Meta object code from reading C++ file 'wdgGraphHist.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/SVStatPanel/src/wdgGraphHist.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wdgGraphHist.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_wdgGraphHist_t {
    QByteArrayData data[5];
    char stringdata0[44];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_wdgGraphHist_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_wdgGraphHist_t qt_meta_stringdata_wdgGraphHist = {
    {
QT_MOC_LITERAL(0, 0, 12), // "wdgGraphHist"
QT_MOC_LITERAL(1, 13, 14), // "selValueChange"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 10), // "setTargPos"
QT_MOC_LITERAL(4, 40, 3) // "pos"

    },
    "wdgGraphHist\0selValueChange\0\0setTargPos\0"
    "pos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_wdgGraphHist[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    4,

       0        // eod
};

void wdgGraphHist::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        wdgGraphHist *_t = static_cast<wdgGraphHist *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selValueChange(); break;
        case 1: _t->setTargPos((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (wdgGraphHist::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&wdgGraphHist::selValueChange)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject wdgGraphHist::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_wdgGraphHist.data,
      qt_meta_data_wdgGraphHist,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *wdgGraphHist::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *wdgGraphHist::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_wdgGraphHist.stringdata0))
        return static_cast<void*>(const_cast< wdgGraphHist*>(this));
    return QWidget::qt_metacast(_clname);
}

int wdgGraphHist::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void wdgGraphHist::selValueChange()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
