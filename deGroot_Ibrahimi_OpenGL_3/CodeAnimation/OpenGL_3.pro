QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGL_3
TEMPLATE = app

CONFIG += c++14

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mainview.cpp \
    mesh.cpp \
    scene.cpp \
    texture.cpp \
    user_input.cpp \
    model.cpp \
    utility.cpp

HEADERS += \
    mainwindow.h \
    mainview.h \
    mesh.h \
    model.h \
    scene.h \
    texture.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc
