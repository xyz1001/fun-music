#-------------------------------------------------
#
# Project created by QtCreator 2016-05-14T09:29:20
#
#-------------------------------------------------

QT += core gui
QT += network
QT += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fun-music
TEMPLATE = app

CONFIG += C++11

SOURCES += src/main.cpp\
           src/old/mainwindow.cpp \
           src/old/mp3getter.cpp \
           src/old/picturegetter.cpp \
           src/old/datagetter.cpp \
           src/old/clipboardmonitor.cpp \
           src/old/settings.cpp \
           src/old/options.cpp \
           src/old/guidance.cpp \
           src/old/jsongetter.cpp \
           src/old/handlerwithjson.cpp \
           src/old/albumjsonparser.cpp \
           src/old/detailjsonparser.cpp \
           src/old/jsonparser.cpp \
           src/old/networkaccess.cpp \
           src/old/register.cpp \
           src/old/devicecheck.cpp

HEADERS  += src/old/mainwindow.h \
            src/old/mp3getter.h \
            src/old/picturegetter.h \
            src/old/datagetter.h \
            src/old/clipboardmonitor.h \
            src/old/settings.h \
            src/old/options.h \
            src/old/musicinfo.h \
            src/old/guidance.h \
            src/old/jsongetter.h \
            src/old/handlerwithjson.h \
            src/old/albumjsonparser.h \
            src/old/detailjsonparser.h \
            src/old/jsonparser.h \
            src/old/constant.h \
            src/old/networkaccess.h \
            src/old/register.h \
            src/old/devicecheck.h

FORMS += src/old/mainwindow.ui \
         src/old/options.ui \
         src/old/guidance.ui \
         src/old/register.ui

RESOURCES += res/image.qrc \
             res/language.qrc \
             res/qss.qrc

TRANSLATIONS += res/language/ts/zh_CN.ts

VERSION = 3.0.1

RC_ICONS = icon.ico
