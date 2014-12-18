#-------------------------------------------------
#
# Project created by QtCreator 2014-10-25T13:30:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageShop
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    filters.cpp \
    sliders.cpp \
    worker.cpp \
    imageview.cpp \
    settingsform.cpp

HEADERS  += mainwindow.h \
    filters.h \
    sliders.h \
    worker.h \
    imageview.h \
    settingsform.h

FORMS    += mainwindow.ui \
    settingsform.ui

RESOURCES += \
    resource.qrc
