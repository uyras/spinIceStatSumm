#-------------------------------------------------
#
# Project created by QtCreator 2015-01-20T14:11:08
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = spinIceStatSumm
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

LIBS += -L$$PWD/../partsEngine/ -lPartsEngine
INCLUDEPATH += $$PWD/../partsEngine
DEPENDPATH += $$PWD/../partsEngine
PRE_TARGETDEPS += $$PWD/../partsEngine/libPartsEngine.a

DISTFILES += \
    checkYourself.nb \
    statSumm2Parts.nb \
    statSumm12Parts.nb \
    README.md \
    res/statSumm2Parts.png \
    res/statSumm12Parts.png
