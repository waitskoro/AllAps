QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    binparser.cpp \
    graph/dsp.cpp \
    graph/graphmanager.cpp \
    graph/graphtracer.cpp \
    graph/markersplotter.cpp \
    graph/minfft/minfft.c \
    graph/plotter.cpp \
    graph/powerplotter.cpp \
    graph/spectrogram.cpp \
    gui/frames/frameiq.cpp \
    gui/frames/framepower.cpp \
    gui/frames/framesettings.cpp \
    gui/frames/framespector.cpp \
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
    graph/dsp.h \
    graph/graphmanager.h \
    graph/graphtracer.h \
    graph/markersplotter.h \
    graph/minfft/minfft.h \
    graph/plotter.h \
    graph/powerplotter.h \
    graph/spectrogram.h \
    gui/frames/commonfunctions.h \
    gui/frames/frameiq.h \
    gui/frames/framepower.h \
    gui/frames/framesettings.h \
    gui/frames/framespector.h \
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
    gui/frames/frameiq.ui \
    gui/frames/framepower.ui \
    gui/frames/framesettings.ui \
    gui/frames/framespector.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
