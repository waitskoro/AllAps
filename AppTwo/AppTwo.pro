QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gui/connectionheader.cpp \
    gui/connectionheaderblock.cpp \
    main.cpp \
    gui/mainwindow.cpp \
    server/sequentialidprovider.cpp \
    server/tcpserver.cpp \
    server/tcpsocket.cpp

HEADERS += \
    gui/connectionheader.h \
    gui/connectionheaderblock.h \
    gui/mainwindow.h \
    server/packet.h \
    server/tcpserver.h \
    server/tcpsocket.h
    server/sequentialidprovider.h \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
