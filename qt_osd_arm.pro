######################################################################
# Automatically generated by qmake (2.01a) ?? 9? 14 14:36:34 2017
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH  += . src
INCLUDEPATH += . src

# Input

HEADERS +=  src/Button.h \
            src/jpegenc.h \
            src/mainwindow.h \
            src/qbattery.h \
            src/qgximage.h \
            src/qt.h \
            src/qv4l2.h \
            src/mainmenu.h \
            src/backplay.h \
            src/MP4Encoder.h

FORMS   +=  src/mainwindow.ui

SOURCES +=  src/Button.cpp \
            src/jpegenc.c \
            src/main.cpp \
            src/mainwindow.cpp \
            src/qbattery.cpp \
            src/qgximage.cpp \
            src/qv4l2.cpp \
            src/mainmenu.cpp \
            src/backplay.cpp \
            src/MP4Encoder.c

RESOURCES +=src/qt_osd_res.qrc

QT += declarative

