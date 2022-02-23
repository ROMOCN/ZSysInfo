QT       += core gui
QT += widgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Controls/my_groupbox.cpp \
    Controls/my_showwidget.cpp \
    Controls/my_tablegroup.cpp \
    Tools/tool_hash.cpp \
    Tools/tool_system.cpp \
    include/qcustomplot.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Controls/my_edit.h \
    Controls/my_groupbox.h \
    Controls/my_showwidget.h \
    Controls/my_tablegroup.h \
    Tools/tool_cpuTemperature.h \
    Tools/tool_edge.h \
    Tools/tool_hash.h \
    Tools/tool_nv.h \
    Tools/tool_system.h \
    Tools/tool_value.h \
    include/qcustomplot.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/../include
LIBS += -L$$PWD/include/  -lnvml
#-lnvapi  -lAdvAPI32
INCLUDEPATH += $$PWD/../include/R495-developer/
LIBS += -L$$PWD/include/R495-developer/  -lnvapi64
RESOURCES += \
    resource.qrc

DISTFILES += \
    logo.rc

RC_FILE += logo.rc
