#-------------------------------------------------
#
# Project created by QtCreator 2012-05-21T08:24:01
#
#-------------------------------------------------

QT       += core gui network

TARGET = wdolm
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    computer.cpp \
    loginrequestform.cpp

HEADERS  += mainwindow.h \
    computer.h \
    loginrequestform.h

FORMS    += mainwindow.ui \
    loginrequestform.ui

OTHER_FILES += \
    images/computer.png

RESOURCES += \
    resources.qrc
