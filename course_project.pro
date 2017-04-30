#-------------------------------------------------
#
# Project created by QtCreator 2017-04-21T08:51:59
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += widgets

QT += printsupport

CONFIG += c++11

#LIBS += -lncreport -L /opt/NCReport2.20.1.x64.Qt5.5.1.eval/lib -L/usr/local/bin
#LIBS += -L /opt/NCReport2.20.1.x64.Qt5.5.1.eval/lib/ -lNCReport2
LIBS += /opt/NCReport2.20.1.x64.Qt5.5.1.eval/lib/libNCReport.so.2.20.1
#LIBS += /opt/NCReport2.20.1.x64.Qt5.5.1.eval/lib/libNCReportPreviewWidget.so
#LIBS += /opt/NCReport2.20.1.x64.Qt5.5.1.eval/lib/libNCReport.so
INCLUDEPATH += /opt/NCReport2.20.1.x64.Qt5.5.1.eval/include
#LIBS += D:\IASA\BD\NCReport\2.20.2.VS2015.Qt5.8.0.eval\lib\ncreport2.lib
#INCLUDEPATH += D:\IASA\BD\NCReport\2.20.2.VS2015.Qt5.8.0.eval\include

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
    orderwindow.cpp \
    spinboxdelegate.cpp \
    moveorder.cpp

HEADERS  += mainwindow.h \
    loginwindow.h \
    addproduct.h \
    tools.h \
    enhancedtablewidget.h \
    productpicker.h \
    contractorwindow.h \
    orderwindow.h \
    spinboxdelegate.h \
    moveorder.h

FORMS    += mainwindow.ui \
    loginwindow.ui \
    addproduct.ui \
    productpicker.ui \
    contractorwindow.ui \
    orderwindow.ui \
    moveorder.ui
