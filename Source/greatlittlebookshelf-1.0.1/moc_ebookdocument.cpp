/****************************************************************************
** Meta object code from reading C++ file 'ebookdocument.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ebookdocument.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ebookdocument.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_eBookDocument[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   15,   14,   14, 0x05,
      52,   47,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      81,   47,   74,   14, 0x0a,
     106,   47,   74,   14, 0x0a,
     144,  135,  130,   14, 0x0a,
     170,  165,   14,   14, 0x0a,
     183,   14,   14,   14, 0x2a,
     199,  193,   14,   14, 0x0a,
     215,  165,   14,   14, 0x0a,
     229,   14,   14,   14, 0x2a,
     240,  165,   14,   14, 0x0a,
     257,   14,   14,   14, 0x2a,

       0        // eod
};

static const char qt_meta_stringdata_eBookDocument[] = {
    "eBookDocument\0\0trenutnaStrana\0"
    "pageChanged(int)\0text\0textSelected(QString)\0"
    "QRectF\0searchBackwards(QString)\0"
    "searchForwards(QString)\0bool\0filePath\0"
    "setDocument(QString)\0page\0setPage(int)\0"
    "setPage()\0scale\0setScale(qreal)\0"
    "showPage(int)\0showPage()\0showPageFit(int)\0"
    "showPageFit()\0"
};

void eBookDocument::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        eBookDocument *_t = static_cast<eBookDocument *>(_o);
        switch (_id) {
        case 0: _t->pageChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->textSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: { QRectF _r = _t->searchBackwards((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QRectF*>(_a[0]) = _r; }  break;
        case 3: { QRectF _r = _t->searchForwards((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QRectF*>(_a[0]) = _r; }  break;
        case 4: { bool _r = _t->setDocument((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: _t->setPage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setPage(); break;
        case 7: _t->setScale((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 8: _t->showPage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->showPage(); break;
        case 10: _t->showPageFit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->showPageFit(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData eBookDocument::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject eBookDocument::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_eBookDocument,
      qt_meta_data_eBookDocument, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &eBookDocument::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *eBookDocument::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *eBookDocument::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_eBookDocument))
        return static_cast<void*>(const_cast< eBookDocument*>(this));
    return QLabel::qt_metacast(_clname);
}

int eBookDocument::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void eBookDocument::pageChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void eBookDocument::textSelected(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
