#-------------------------------------------------
#
# Project created by QtCreator 2021-09-15T11:38:01
#
#-------------------------------------------------

QT       += core gui qml quickwidgets location location-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Aerophotography
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    scene.cpp \
    planemenu.cpp \
    myview.cpp \
    graphicsviewzoom.cpp \
    plane.cpp

HEADERS += \
        mainwindow.h \
    scene.h \
    planemenu.h \
    myview.h \
    graphicsviewzoom.h \
    plane.h
RESOURCES += qml.qrc \
    images.qrc
FORMS += \
        mainwindow.ui \
    planemenu.ui
CONFIG += qmltypes
QML_IMPORT_NAME = io.qt.examples.backend
QML_IMPORT_MAJOR_VERSION = 1
DISTFILES +=
