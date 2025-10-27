/****************************************************************************
** Meta object code from reading C++ file 'mainwidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../AppOne/gui/mainwidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_View__MainWidget_t {
    uint offsetsAndSizes[36];
    char stringdata0[17];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[17];
    char stringdata5[17];
    char stringdata6[25];
    char stringdata7[11];
    char stringdata8[32];
    char stringdata9[7];
    char stringdata10[13];
    char stringdata11[14];
    char stringdata12[27];
    char stringdata13[19];
    char stringdata14[7];
    char stringdata15[28];
    char stringdata16[20];
    char stringdata17[9];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_View__MainWidget_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_View__MainWidget_t qt_meta_stringdata_View__MainWidget = {
    {
        QT_MOC_LITERAL(0, 16),  // "View::MainWidget"
        QT_MOC_LITERAL(17, 9),  // "reconnect"
        QT_MOC_LITERAL(27, 0),  // ""
        QT_MOC_LITERAL(28, 10),  // "disconnect"
        QT_MOC_LITERAL(39, 16),  // "sendStopingPlans"
        QT_MOC_LITERAL(56, 16),  // "sendRequestState"
        QT_MOC_LITERAL(73, 24),  // "sendRequestTrackingPlans"
        QT_MOC_LITERAL(98, 10),  // "sendTarget"
        QT_MOC_LITERAL(109, 31),  // "Application::TargetDesignations"
        QT_MOC_LITERAL(141, 6),  // "target"
        QT_MOC_LITERAL(148, 12),  // "recieveState"
        QT_MOC_LITERAL(161, 13),  // "StateMessage&"
        QT_MOC_LITERAL(175, 26),  // "executedTheCommandRecevied"
        QT_MOC_LITERAL(202, 18),  // "ExecutedTheCommand"
        QT_MOC_LITERAL(221, 6),  // "result"
        QT_MOC_LITERAL(228, 27),  // "dataChannelMessagesRecieved"
        QT_MOC_LITERAL(256, 19),  // "DataChannelMessage&"
        QT_MOC_LITERAL(276, 8)   // "messages"
    },
    "View::MainWidget",
    "reconnect",
    "",
    "disconnect",
    "sendStopingPlans",
    "sendRequestState",
    "sendRequestTrackingPlans",
    "sendTarget",
    "Application::TargetDesignations",
    "target",
    "recieveState",
    "StateMessage&",
    "executedTheCommandRecevied",
    "ExecutedTheCommand",
    "result",
    "dataChannelMessagesRecieved",
    "DataChannelMessage&",
    "messages"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_View__MainWidget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x06,    1 /* Public */,
       3,    0,   69,    2, 0x06,    2 /* Public */,
       4,    0,   70,    2, 0x06,    3 /* Public */,
       5,    0,   71,    2, 0x06,    4 /* Public */,
       6,    0,   72,    2, 0x06,    5 /* Public */,
       7,    1,   73,    2, 0x06,    6 /* Public */,
      10,    1,   76,    2, 0x06,    8 /* Public */,
      12,    1,   79,    2, 0x06,   10 /* Public */,
      15,    1,   82,    2, 0x06,   12 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,    2,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 16,   17,

       0        // eod
};

Q_CONSTINIT const QMetaObject View::MainWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_View__MainWidget.offsetsAndSizes,
    qt_meta_data_View__MainWidget,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_View__MainWidget_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWidget, std::true_type>,
        // method 'reconnect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'disconnect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendStopingPlans'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendRequestState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendRequestTrackingPlans'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendTarget'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Application::TargetDesignations, std::false_type>,
        // method 'recieveState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<StateMessage &, std::false_type>,
        // method 'executedTheCommandRecevied'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<ExecutedTheCommand, std::false_type>,
        // method 'dataChannelMessagesRecieved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<DataChannelMessage &, std::false_type>
    >,
    nullptr
} };

void View::MainWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->reconnect(); break;
        case 1: _t->disconnect(); break;
        case 2: _t->sendStopingPlans(); break;
        case 3: _t->sendRequestState(); break;
        case 4: _t->sendRequestTrackingPlans(); break;
        case 5: _t->sendTarget((*reinterpret_cast< std::add_pointer_t<Application::TargetDesignations>>(_a[1]))); break;
        case 6: _t->recieveState((*reinterpret_cast< std::add_pointer_t<StateMessage&>>(_a[1]))); break;
        case 7: _t->executedTheCommandRecevied((*reinterpret_cast< std::add_pointer_t<ExecutedTheCommand>>(_a[1]))); break;
        case 8: _t->dataChannelMessagesRecieved((*reinterpret_cast< std::add_pointer_t<DataChannelMessage&>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWidget::*)();
            if (_t _q_method = &MainWidget::reconnect; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWidget::*)();
            if (_t _q_method = &MainWidget::disconnect; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWidget::*)();
            if (_t _q_method = &MainWidget::sendStopingPlans; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainWidget::*)();
            if (_t _q_method = &MainWidget::sendRequestState; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MainWidget::*)();
            if (_t _q_method = &MainWidget::sendRequestTrackingPlans; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (MainWidget::*)(Application::TargetDesignations );
            if (_t _q_method = &MainWidget::sendTarget; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (MainWidget::*)(StateMessage & );
            if (_t _q_method = &MainWidget::recieveState; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (MainWidget::*)(ExecutedTheCommand );
            if (_t _q_method = &MainWidget::executedTheCommandRecevied; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (MainWidget::*)(DataChannelMessage & );
            if (_t _q_method = &MainWidget::dataChannelMessagesRecieved; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject *View::MainWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *View::MainWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_View__MainWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int View::MainWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void View::MainWidget::reconnect()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void View::MainWidget::disconnect()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void View::MainWidget::sendStopingPlans()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void View::MainWidget::sendRequestState()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void View::MainWidget::sendRequestTrackingPlans()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void View::MainWidget::sendTarget(Application::TargetDesignations _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void View::MainWidget::recieveState(StateMessage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void View::MainWidget::executedTheCommandRecevied(ExecutedTheCommand _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void View::MainWidget::dataChannelMessagesRecieved(DataChannelMessage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
