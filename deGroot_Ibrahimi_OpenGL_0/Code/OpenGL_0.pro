QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGL_0
TEMPLATE = app

CONFIG += c++14

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mainview.cpp \
    user_input.cpp

HEADERS += \
    Vertex.h \
    mainwindow.h \
    mainview.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc
