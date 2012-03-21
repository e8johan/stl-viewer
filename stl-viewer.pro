#-------------------------------------------------
#
# Project created by QtCreator 2012-03-20T14:37:01
#
#-------------------------------------------------

QT       += core gui opengl

LIBS += -lGLU

TARGET = stl-viewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    stlview.cpp

HEADERS  += mainwindow.h \
    stlview.h

FORMS    += mainwindow.ui
