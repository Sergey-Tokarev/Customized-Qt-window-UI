#-------------------------------------------------
#
# Project created by QtCreator 2015-04-25T18:51:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TransparentWindow
TEMPLATE = app


SOURCES += main.cpp\
        transparentmainwindow.cpp \
    customwindow.cpp \
    switcherbutton.cpp

HEADERS  += transparentmainwindow.h \
    customwindow.h \
    switcherbutton.h

FORMS    += transparentmainwindow.ui \
    customwindow.ui \
    switcherbutton.ui

RESOURCES += \
    transparentwindow.qrc
