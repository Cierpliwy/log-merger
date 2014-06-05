#-------------------------------------------------
#
# Project created by QtCreator 2014-04-19T22:14:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MBI
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES +=\
        MainWindow.cpp \
        Main.cpp \
    File.cpp \
    FileHelper.cpp \
    NeedlemanWunschAlgorithm.cpp

HEADERS  += MainWindow.h \
    File.h \
    FileHelper.h \
    TokenGraph.h \
    Token.h \
    NeedlemanWunschAlgorithm.h \
    NWASettings.h

FORMS    += MainWindow.ui
