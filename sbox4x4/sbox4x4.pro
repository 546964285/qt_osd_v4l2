#-------------------------------------------------
#
# Project created by QtCreator 2016-04-21T10:56:50
#
#-------------------------------------------------

TARGET = sbox4x4
TEMPLATE = lib
CONFIG += plugin

PLUGIN_FOR = qt
PLUGIN_TYPE = kbddrivers

DEFINES += SBOX4X4_LIBRARY

SOURCES += sbox4x4handler.cpp \
    sbox4x4plugin.cpp

HEADERS += sbox4x4handler.h \
    sbox4x4plugin.h \
    sbox4x4handler_global.h
