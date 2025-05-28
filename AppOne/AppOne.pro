QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gui/featuremanagement/targetdesignationtable.cpp \
    gui/featuremanagement/targetdesignationwidget.cpp \
    gui/ipconnectionsetupwidget.cpp \
    gui/loader.cpp \
    gui/mainwidget.cpp \
    gui/menuwidget.cpp \
    gui/plansmanagment/planslist.cpp \
    gui/plansmanagment/planslistdelegate.cpp \
    gui/planswidget.cpp \
    gui/state/stateworkingwidget.cpp \
    gui/stopingplans/stopingplanswidget.cpp \
    gui/trackingplans/channeldatadelegate.cpp \
    gui/trackingplans/channeldatalist.cpp \
    gui/trackingplans/trackingplanswidget.cpp \
    main.cpp \
    src/connection/connectiondatasaving.cpp \
    src/connection/ipconnectionsetupmanager.cpp \
    src/featuremanagment/targetdesignationmodel.cpp \
    src/mainmanger.cpp \
    src/socket/sequentialidprovider.cpp \
    src/socket/tcpsocket.cpp \
    src/socket/tcpsocketmanager.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    gui/featuremanagement/targetdesignationtable.h \
    gui/featuremanagement/targetdesignationwidget.h \
    gui/ipconnectionsetupwidget.h \
    gui/loader.h \
    gui/mainwidget.h \
    gui/menuwidget.h \
    gui/plansmanagment/planslist.h \
    gui/plansmanagment/planslistdelegate.h \
    gui/planswidget.h \
    gui/state/stateworkingwidget.h \
    gui/stopingplans/stopingplanswidget.h \
    gui/trackingplans/channeldatadelegate.h \
    gui/trackingplans/channeldatalist.h \
    gui/trackingplans/trackingplanswidget.h \
    src/connection/connectiondatasaving.h \
    src/connection/ipconnectionsetupmanager.h \
    src/contants.h \
    src/featuremanagment/targetdesignationmodel.h \
    src/featuremanagment/targetdesignationsinfo.h \
    src/mainmanger.h \
    src/messagesinfo.h \
    src/socket/sequentialidprovider.h \
    src/socket/socketinfo.h \
    src/socket/tcpsocket.h \
    src/socket/tcpsocketmanager.h

DISTFILES +=
