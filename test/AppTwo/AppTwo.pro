QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    binparser.cpp \
    graph/dftplotter.cpp \
    graph/dsp.cpp \
    graph/graphmanager.cpp \
    graph/graphtracer.cpp \
    graph/minfft/minfft.c \
    graph/powerplotter.cpp \
    graph/timeplotter.cpp \
    info/infolistdelegate.cpp \
    info/infomanager.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindowmanager.cpp \
    qcustomplot/qcustomplot.cpp \
    src/sequentialidprovider.cpp \
    src/tcpmanager.cpp

HEADERS += \
    binparser.h \
    graph/dftplotter.h \
    graph/dsp.h \
    graph/graphmanager.h \
    graph/graphtracer.h \
    graph/minfft/minfft.h \
    graph/powerplotter.h \
    graph/timeplotter.h \
    info/enums.h \
    info/infolistdelegate.h \
    info/infomanager.h \
    mainwindow.h \
    mainwindowmanager.h \
    qcustomplot/qcustomplot.h \
    src/message.h \
    src/sequentialidprovider.h \
    src/tcpmanager.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
