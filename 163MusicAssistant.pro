#-------------------------------------------------
#
# Project created by QtCreator 2016-05-14T09:29:20
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 163MusicAssistant
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    jsonhandler.cpp \
    jsongetter.cpp \
    mp3getter.cpp \
    picturegetter.cpp \
    htmlgetter.cpp \
    htmlhandler.cpp \
    handlerwithjson.cpp \
    datagetter.cpp \
    handlerwithhtml.cpp \
    clipboardmonitor.cpp \
    settings.cpp \
    options.cpp

HEADERS  += mainwindow.h \
    jsonhandler.h \
    jsongetter.h \
    mp3getter.h \
    picturegetter.h \
    htmlgetter.h \
    htmlhandler.h \
    handlerwithjson.h \
    datagetter.h \
    handlerwithhtml.h \
    clipboardmonitor.h \
    settings.h \
    options.h

FORMS    += mainwindow.ui \
    options.ui \
    androidmainwindow.ui \
    androidoptions.ui

CONFIG   += C++11

TRANSLATIONS    += myI18N_zh_CN.ts

RESOURCES += \
    res.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
