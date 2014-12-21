#-------------------------------------------------
#
# Project created by QtCreator 2014-12-19T21:37:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PcapReader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pcappacktablemodel.cpp \
    pcap_loader.cpp

HEADERS  += mainwindow.h \
    pcappacktablemodel.h \
    pcap_loader.h

FORMS    += mainwindow.ui
