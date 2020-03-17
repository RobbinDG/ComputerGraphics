QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGL_3
TEMPLATE = app

CONFIG += c++14

SOURCES += \
    animatedmesh.cpp \
    animation.cpp \
    backandforthanimation.cpp \
    bounceanimation.cpp \
    drawable.cpp \
    globalrotateanimation.cpp \
    localrotateanimation.cpp \
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
    animatedmesh.h \
    animation.h \
    animations.h \
    backandforthanimation.h \
    bounceanimation.h \
    drawable.h \
    globalrotateanimation.h \
    localrotateanimation.h \
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
