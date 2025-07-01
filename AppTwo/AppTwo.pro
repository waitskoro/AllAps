QT       += core gui network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gui/common/customcombobox.cpp \
    gui/graph/graphwidget.cpp \
    gui/graph/plotter.cpp \
    gui/graph/qcustomplot.cpp \
    gui/infolist.cpp \
    gui/infolistdelegate.cpp \
    gui/infowidget.cpp \
    gui/mainwindow.cpp \
    gui/reportsmodel.cpp \
    gui/reportsstable.cpp \
    gui/serverconnectingwidget.cpp \
    main.cpp \
    mainmanager.cpp \
    src/csvparser.cpp \
    src/graph/minfft.c \
    src/sequentialidprovider.cpp \
    src/tcpmanager.cpp

HEADERS += \
    common/enums.h \
    common/messages.h \
    gui/common/customcombobox.h \
    gui/graph/graphwidget.h \
    gui/graph/plotter.h \
    gui/graph/qcustomplot.h \
    gui/infolist.h \
    gui/infolistdelegate.h \
    gui/infowidget.h \
    gui/mainwindow.h \
    gui/reportsmodel.h \
    gui/reportsstable.h \
    gui/serverconnectingwidget.h \
    mainmanager.h \
    src/csvparser.h \
    src/graph/minfft.h \
    src/graph/spectrumanalyzer.h \
    src/sequentialidprovider.h \
    src/tcpmanager.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    gui/resources.qrc
