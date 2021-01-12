#-------------------------------------------------
#
# Project created by QtCreator 2014-06-16T13:13:39
#
#-------------------------------------------------

QT       += core gui widgets xml

TARGET = xvpSample
TEMPLATE = lib
CONFIG += plugin

include(xvpSample.pri)

SOURCES += sampleplugin.cpp \
    NegativeChnbl.cpp \
    BayerPseudoColorsChnbl.cpp \
    MeanGrayMeasForm.cpp \
    FlipImageCnbl.cpp \
    FlipConfigurationWgt.cpp \
    CameraTriggerForm.cpp

HEADERS += sampleplugin.h \
    NegativeChnbl.h \
    BayerPseudoColorsChnbl.h \
    MeanGrayMeasForm.h \
    FlipImageCnbl.h \
    FlipConfigurationWgt.h \
    CameraTriggerForm.h

FORMS += \
    MeanGrayMeasForm.ui \
    FlipConfigurationWgt.ui \
    CameraTriggerForm.ui

RESOURCES += \
    xvpSample.qrc
