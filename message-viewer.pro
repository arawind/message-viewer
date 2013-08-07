#-------------------------------------------------
#
# Project created by QtCreator 2013-06-06T21:46:24
#
#-------------------------------------------------

QT       += core gui
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = message-viewer
TEMPLATE = app


SOURCES += main.cpp\
        messageviewer.cpp \
    csvparser.cpp \
    vcfparser.cpp \
    fbparser.cpp

HEADERS  += messageviewer.h \
    csvparser.h \
    vcfparser.h \
    fbparser.h
