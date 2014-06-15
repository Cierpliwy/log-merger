#-------------------------------------------------
#
# Project created by QtCreator 2014-06-15T16:52:04
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    test/test_main.cpp \
    test/nwa_test.cpp \
    src/NeedlemanWunschAlgorithm.cpp \
    src/FileHelper.cpp \
    src/File.cpp \
    test/file_test.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    test/nwa_test.h \
    test/file_test.h
