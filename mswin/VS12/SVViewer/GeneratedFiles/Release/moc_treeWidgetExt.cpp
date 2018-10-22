/****************************************************************************
** Meta object code from reading C++ file 'treeWidgetExt.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/SVViewer/src/treeWidgetExt.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'treeWidgetExt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_treeWidgetExt_t {
    QByteArrayData data[1];
    char stringdata0[14];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_treeWidgetExt_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_treeWidgetExt_t qt_meta_stringdata_treeWidgetExt = {
    {
QT_MOC_LITERAL(0, 0, 13) // "treeWidgetExt"

    },
    "treeWidgetExt"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_treeWidgetExt[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void treeWidgetExt::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject treeWidgetExt::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_treeWidgetExt.data,
      qt_meta_data_treeWidgetExt,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *treeWidgetExt::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *treeWidgetExt::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_treeWidgetExt.stringdata0))
        return static_cast<void*>(const_cast< treeWidgetExt*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int treeWidgetExt::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
