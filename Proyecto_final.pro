QT += core gui widgets

CONFIG += c++17

TARGET   = Proyecto_final
TEMPLATE = app

SOURCES += \
    barco.cpp \
    main.cpp \
    ola.cpp \
    viento.cpp \
    widget.cpp \
    nivel2oscuridad.cpp

HEADERS += \
    barco.h \
    ola.h \
    viento.h \
    widget.h \
    nivel2oscuridad.h \
    agentekraken.h

FORMS += \
    widget.ui

RESOURCES += \
    fondo.qrc \
    fondo2.qrc
