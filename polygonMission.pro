#-------------------------------------------------
#
# Project created by QtCreator 2016-04-26T11:33:23
#
#-------------------------------------------------

QT       += core gui \
            positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = polygonMission
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    wayPointLine.cpp \
    calculateGeometry.cpp \
    polygonField.cpp

HEADERS  += mainwindow.h \
    wayPointLine.h \
    calculateGeometry.h \
    polygonField.h
