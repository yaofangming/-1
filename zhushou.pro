#-------------------------------------------------
#
# Project created by QtCreator 2019-11-23T14:52:06
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += sql
QT       +=  printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zhushou
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    qcustomplot.cpp

HEADERS  += widget.h \
    qcustomplot.h

FORMS    += widget.ui

CONFIG +=C++11
