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
    src/MainWindow.cpp \
    src/Main.cpp \
    src/File.cpp \
    src/FileHelper.cpp \
    src/NeedlemanWunschAlgorithm.cpp

HEADERS  +=\
    src/MainWindow.h \
    src/File.h \
    src/FileHelper.h \
    src/TokenGraph.h \
    src/Token.h \
    src/NeedlemanWunschAlgorithm.h \
    src/NWASettings.h

FORMS    += src/MainWindow.ui
