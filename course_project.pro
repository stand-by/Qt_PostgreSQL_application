#-------------------------------------------------
#
# Project created by QtCreator 2017-04-21T08:51:59
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += widgets

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = course_project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    loginwindow.cpp \
    addproduct.cpp \
    enhancedtablewidget.cpp \
    productpicker.cpp \
    contractorwindow.cpp \
    orderwindow.cpp

HEADERS  += mainwindow.h \
    loginwindow.h \
    addproduct.h \
    tools.h \
    enhancedtablewidget.h \
    productpicker.h \
    contractorwindow.h \
    orderwindow.h

FORMS    += mainwindow.ui \
    loginwindow.ui \
    addproduct.ui \
    productpicker.ui \
    contractorwindow.ui \
    orderwindow.ui
