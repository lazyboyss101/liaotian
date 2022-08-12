/****************************************************************************
** Meta object code from reading C++ file 'mychatdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MyChat/mychatdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mychatdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MyChatDialog_t {
    QByteArrayData data[11];
    char stringdata0[115];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyChatDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyChatDialog_t qt_meta_stringdata_MyChatDialog = {
    {
QT_MOC_LITERAL(0, 0, 12), // "MyChatDialog"
QT_MOC_LITERAL(1, 13, 13), // "SIG_AddFriend"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 9), // "SIG_Close"
QT_MOC_LITERAL(4, 38, 15), // "SIG_SendChatMsg"
QT_MOC_LITERAL(5, 54, 2), // "id"
QT_MOC_LITERAL(6, 57, 7), // "content"
QT_MOC_LITERAL(7, 65, 19), // "on_pb_tool1_clicked"
QT_MOC_LITERAL(8, 85, 13), // "slot_dealMenu"
QT_MOC_LITERAL(9, 99, 8), // "QAction*"
QT_MOC_LITERAL(10, 108, 6) // "action"

    },
    "MyChatDialog\0SIG_AddFriend\0\0SIG_Close\0"
    "SIG_SendChatMsg\0id\0content\0"
    "on_pb_tool1_clicked\0slot_dealMenu\0"
    "QAction*\0action"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyChatDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,
       4,    2,   41,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   46,    2, 0x08 /* Private */,
       8,    1,   47,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void MyChatDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyChatDialog *_t = static_cast<MyChatDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SIG_AddFriend(); break;
        case 1: _t->SIG_Close(); break;
        case 2: _t->SIG_SendChatMsg((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->on_pb_tool1_clicked(); break;
        case 4: _t->slot_dealMenu((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MyChatDialog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyChatDialog::SIG_AddFriend)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MyChatDialog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyChatDialog::SIG_Close)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (MyChatDialog::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyChatDialog::SIG_SendChatMsg)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject MyChatDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MyChatDialog.data,
      qt_meta_data_MyChatDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyChatDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyChatDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyChatDialog.stringdata0))
        return static_cast<void*>(const_cast< MyChatDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int MyChatDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void MyChatDialog::SIG_AddFriend()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void MyChatDialog::SIG_Close()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void MyChatDialog::SIG_SendChatMsg(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
