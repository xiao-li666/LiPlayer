/****************************************************************************
** Meta object code from reading C++ file 'Player.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../Player.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Player.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Player_t {
    QByteArrayData data[23];
    char stringdata0[251];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Player_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Player_t qt_meta_stringdata_Player = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Player"
QT_MOC_LITERAL(1, 7, 11), // "btnOpenFile"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 9), // "btnIsPlay"
QT_MOC_LITERAL(4, 30, 14), // "updatePosition"
QT_MOC_LITERAL(5, 45, 3), // "pts"
QT_MOC_LITERAL(6, 49, 7), // "totalMs"
QT_MOC_LITERAL(7, 57, 13), // "sliderPressed"
QT_MOC_LITERAL(8, 71, 14), // "sliderReleased"
QT_MOC_LITERAL(9, 86, 11), // "videoSlider"
QT_MOC_LITERAL(10, 98, 5), // "value"
QT_MOC_LITERAL(11, 104, 8), // "btnAudio"
QT_MOC_LITERAL(12, 113, 14), // "onSpeedChanged"
QT_MOC_LITERAL(13, 128, 8), // "QAction*"
QT_MOC_LITERAL(14, 137, 6), // "action"
QT_MOC_LITERAL(15, 144, 15), // "btnPlayListHint"
QT_MOC_LITERAL(16, 160, 22), // "playFromPlayListWidget"
QT_MOC_LITERAL(17, 183, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(18, 200, 4), // "item"
QT_MOC_LITERAL(19, 205, 10), // "btnPreSlot"
QT_MOC_LITERAL(20, 216, 11), // "btnNextSlot"
QT_MOC_LITERAL(21, 228, 8), // "OnBtnNet"
QT_MOC_LITERAL(22, 237, 13) // "OnBtnMaxOrMin"

    },
    "Player\0btnOpenFile\0\0btnIsPlay\0"
    "updatePosition\0pts\0totalMs\0sliderPressed\0"
    "sliderReleased\0videoSlider\0value\0"
    "btnAudio\0onSpeedChanged\0QAction*\0"
    "action\0btnPlayListHint\0playFromPlayListWidget\0"
    "QListWidgetItem*\0item\0btnPreSlot\0"
    "btnNextSlot\0OnBtnNet\0OnBtnMaxOrMin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Player[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    2,   86,    2, 0x08 /* Private */,
       7,    0,   91,    2, 0x08 /* Private */,
       8,    0,   92,    2, 0x08 /* Private */,
       9,    1,   93,    2, 0x08 /* Private */,
      11,    0,   96,    2, 0x08 /* Private */,
      12,    1,   97,    2, 0x08 /* Private */,
      15,    0,  100,    2, 0x08 /* Private */,
      16,    1,  101,    2, 0x08 /* Private */,
      19,    0,  104,    2, 0x08 /* Private */,
      20,    0,  105,    2, 0x08 /* Private */,
      21,    0,  106,    2, 0x08 /* Private */,
      22,    0,  107,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Player::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Player *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->btnOpenFile(); break;
        case 1: _t->btnIsPlay(); break;
        case 2: _t->updatePosition((*reinterpret_cast< long long(*)>(_a[1])),(*reinterpret_cast< long long(*)>(_a[2]))); break;
        case 3: _t->sliderPressed(); break;
        case 4: _t->sliderReleased(); break;
        case 5: _t->videoSlider((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->btnAudio(); break;
        case 7: _t->onSpeedChanged((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 8: _t->btnPlayListHint(); break;
        case 9: _t->playFromPlayListWidget((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 10: _t->btnPreSlot(); break;
        case 11: _t->btnNextSlot(); break;
        case 12: _t->OnBtnNet(); break;
        case 13: _t->OnBtnMaxOrMin(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Player::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Player.data,
    qt_meta_data_Player,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Player::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Player::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Player.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Player::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
