QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gui/infolist.cpp \
    gui/infolistdelegate.cpp \
    gui/infowidget.cpp \
    gui/mainwindow.cpp \
    gui/serverconnectingwidget.cpp \
    main.cpp \
    mainmanager.cpp \
    src/sequentialidprovider.cpp \
    src/tcpmanager.cpp \
    src/tcpsocket.cpp

HEADERS += \
    common/enums.h \
    common/messages.h \
    gui/infolist.h \
    gui/infolistdelegate.h \
    gui/infowidget.h \
    gui/mainwindow.h \
    gui/serverconnectingwidget.h \
    mainmanager.h \
    src/sequentialidprovider.h \
    src/tcpmanager.h \
    src/tcpsocket.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    gui/resources.qrc
