/****************************************************************************
** Meta object code from reading C++ file 'watchfolder.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "watchfolder.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'watchfolder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_watchfolder[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x0a,
      29,   12,   12,   12, 0x0a,
      44,   12,   12,   12, 0x0a,
      59,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_watchfolder[] = {
    "watchfolder\0\0writeSettings()\0"
    "readSettings()\0browseFolder()\0"
    "EnableDisable()\0"
};

void watchfolder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        watchfolder *_t = static_cast<watchfolder *>(_o);
        switch (_id) {
        case 0: _t->writeSettings(); break;
        case 1: _t->readSettings(); break;
        case 2: _t->browseFolder(); break;
        case 3: _t->EnableDisable(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData watchfolder::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject watchfolder::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_watchfolder,
      qt_meta_data_watchfolder, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &watchfolder::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *watchfolder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *watchfolder::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_watchfolder))
        return static_cast<void*>(const_cast< watchfolder*>(this));
    return QDialog::qt_metacast(_clname);
}

int watchfolder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
