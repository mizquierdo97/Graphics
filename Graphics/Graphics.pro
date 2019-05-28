#-------------------------------------------------
#
# Project created by QtCreator 2019-02-12T10:38:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Graphics
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    openglwidget.cpp \
    inspectorwidget.cpp \
    object.cpp \
    component.cpp \
    hierarchy.cpp \
    componenttransform.cpp \
    scenewidget.cpp \
    mesh.cpp \
    submesh.cpp \
    vertexformat.cpp \
    componentmesh.cpp \
    camera.cpp \
    input.cpp \
    resources.cpp \
    resourcemesh.cpp \
    resourcetexture.cpp \
    meshwidget.cpp \
    resource.cpp \
    textureswidget.cpp \
    material.cpp \
    transwidget.cpp

HEADERS += \
        mainwindow.h \
    openglwidget.h \
    inspectorwidget.h \
    object.h \
    component.h \
    hierarchy.h \
    componenttransform.h \
    scenewidget.h \
    mesh.h \
    submesh.h \
    vertexformat.h \
    componentmesh.h \
    camera.h \
    input.h \
    resources.h \
    resourcemesh.h \
    resourcetexture.h \
    meshwidget.h \
    resource.h \
    textureswidget.h \
    material.h \
    transwidget.h

FORMS += \
        mainwindow.ui \
    rendering.ui \
    hierarchy.ui \
    meshwidget.ui \
    textureswidget.ui \
    transwidget.ui

#Adds openGL library
QT += opengl
QT += core
QT += 3drender

CONFIG += console

LIBS += -lopengl32

RESOURCES += \
icons.qrc \
    icons.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ThirdParty/Assimp/lib/ -lassimp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ThirdParty/Assimp/lib/ -lassimpd
else:unix: LIBS += -L$$PWD/ThirdParty/Assimp/lib/ -lassimp

INCLUDEPATH += $$PWD/ThirdParty/Assimp/include
DEPENDPATH += $$PWD/ThirdParty/Assimp/include

DISTFILES += \
    shaders/deferred_frag \
    shaders/deferred_vert \
    shaders/shader1_frag \
    shaders/shader1_vert \
    shaders/blur_frag \
    shaders/depthfield_frag
