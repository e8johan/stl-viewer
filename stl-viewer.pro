QT += core gui opengl

# Seems to be necessary on Linux
unix: LIBS += -lGLU

TARGET = stl-viewer
TEMPLATE = app

# Setup an installation target
target.path = /usr/bin
INSTALLS += target

SOURCES += main.cpp\
        mainwindow.cpp \
    stlview.cpp

HEADERS  += mainwindow.h \
    stlview.h

FORMS    += mainwindow.ui
