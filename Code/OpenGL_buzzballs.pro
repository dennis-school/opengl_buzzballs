#-------------------------------------------------
#
# Project created by QtCreator 2017-01-24T12:26:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGL_buzzballs
TEMPLATE = app
CONFIG += c++14

SOURCES += main.cpp\
    mainwindow.cpp \
    mainview.cpp \
    user_input.cpp \
    model.cpp \
    batch.cpp \
    transform.cpp \
    material.cpp \
    animation.cpp

HEADERS  += mainwindow.h \
    mainview.h \
    model.h \
    batch.h \
    transform.h \
    material.h \
    animation.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    shaders/normal_vertshader.glsl \
    shaders/normal_fragshader.glsl \
    shaders/gouraud_fragshader.glsl \
    shaders/gouraud_vertshader.glsl \
    shaders/phong_vertshader.glsl \
    shaders/phong_fragshader.glsl \
    shaders/water_fragshader.glsl \
    shaders/water_vertshader.glsl
