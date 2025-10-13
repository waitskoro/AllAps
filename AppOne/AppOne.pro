QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gui/featuremanagement/coordinatesconverter.cpp \
    gui/featuremanagement/datetarget.cpp \
    gui/featuremanagement/generalinfotarget.cpp \
    gui/featuremanagement/manualtabtargets.cpp \
    gui/featuremanagement/qcustomplot.cpp \
    gui/featuremanagement/rangetabtargets.cpp \
    gui/featuremanagement/spiralgraph.cpp \
    gui/featuremanagement/spiraltabtargets.cpp \
    gui/featuremanagement/suntabtargets.cpp \
    gui/featuremanagement/targetdesignations.cpp \
    gui/featuremanagement/targetdesignationwidget.cpp \
    gui/featuremanagement/test/targetdesignationstest.cpp \
    gui/ipconnectionsetupwidget.cpp \
    gui/loader.cpp \
    gui/mainwidget.cpp \
    gui/menuwidget.cpp \
    gui/plansmanagment/planslist.cpp \
    gui/plansmanagment/planslistdelegate.cpp \
    gui/planswidget.cpp \
    gui/state/cam/camradiatorsdelegate.cpp \
    gui/state/cam/camradiatorslist.cpp \
    gui/state/cam/statecamdelegate.cpp \
    gui/state/cam/statecamlist.cpp \
    gui/state/cam/statecamradiators.cpp \
    gui/state/stateworkingdelegate.cpp \
    gui/state/stateworkinglist.cpp \
    gui/state/stateworkingwidget.cpp \
    gui/stopingplans/stopingplanswidget.cpp \
    gui/trackingchanneldata/channeldatadelegate.cpp \
    gui/trackingchanneldata/channeldatalist.cpp \
    gui/trackingchanneldata/segmentdelegate.cpp \
    gui/trackingchanneldata/segmentslist.cpp \
    gui/trackingchanneldata/targetdesignationsview.cpp \
    gui/trackingchanneldata/targetsview.cpp \
    gui/trackingchanneldata/trackingplanswidget.cpp \
    main.cpp \
    src/connection/connectiondatasaving.cpp \
    src/connection/ipconnectionsetupmanager.cpp \
    src/featuremanagment/suntargetdesignations.cpp \
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
    gui/featuremanagement/coordinatesconverter.h \
    gui/featuremanagement/datetarget.h \
    gui/featuremanagement/generalinfotarget.h \
    gui/featuremanagement/manualtabtargets.h \
    gui/featuremanagement/qcustomplot.h \
    gui/featuremanagement/rangetabtargets.h \
    gui/featuremanagement/spiralgraph.h \
    gui/featuremanagement/spiraltabtargets.h \
    gui/featuremanagement/suntabtargets.h \
    gui/featuremanagement/targetdesignations.h \
    gui/featuremanagement/targetdesignationwidget.h \
    gui/featuremanagement/test/targetdesignationstest.h \
    gui/ipconnectionsetupwidget.h \
    gui/loader.h \
    gui/mainwidget.h \
    gui/menuwidget.h \
    gui/plansmanagment/planslist.h \
    gui/plansmanagment/planslistdelegate.h \
    gui/planswidget.h \
    gui/state/cam/camradiatorsdelegate.h \
    gui/state/cam/camradiatorslist.h \
    gui/state/cam/statecamdelegate.h \
    gui/state/cam/statecamlist.h \
    gui/state/cam/statecamradiators.h \
    gui/state/stateworkingdelegate.h \
    gui/state/stateworkinglist.h \
    gui/state/stateworkingwidget.h \
    gui/stopingplans/stopingplanswidget.h \
    gui/trackingchanneldata/channeldatadelegate.h \
    gui/trackingchanneldata/channeldatalist.h \
    gui/trackingchanneldata/segmentdelegate.h \
    gui/trackingchanneldata/segmentslist.h \
    gui/trackingchanneldata/targetdesignationsview.h \
    gui/trackingchanneldata/targetsview.h \
    gui/trackingchanneldata/trackingplanswidget.h \
    src/channelmessage.h \
    src/common.h \
    src/connection/connectiondatasaving.h \
    src/connection/ipconnectionsetupmanager.h \
    src/contants.h \
    src/featuremanagment/suntargetdesignations.h \
    src/featuremanagment/targetdesignationmodel.h \
    src/featuremanagment/targetdesignationsinfo.h \
    src/mainmanger.h \
    src/messagesinfo.h \
    src/socket/sequentialidprovider.h \
    src/socket/socketinfo.h \
    src/socket/tcpsocket.h \
    src/socket/tcpsocketmanager.h \
    src/statemessage.h

DISTFILES +=

RESOURCES += \
    resources.qrc

FORMS += \
    gui/featuremanagement/test/targetdesignationstest.ui
