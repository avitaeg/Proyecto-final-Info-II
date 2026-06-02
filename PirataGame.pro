QT += core gui widgets multimedia

CONFIG += c++17

TARGET = PirataGame
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/entidad.cpp \
    src/barco.cpp \
    src/capitanbarbarroja.cpp \
    src/obstaculo.cpp \
    src/item.cpp \
    src/corriente.cpp \
    src/nivel.cpp \
    src/nivel1archipielago.cpp \
    src/nivel2oscuridad.cpp \
    src/kraken.cpp \
    src/agentekraken.cpp \
    src/objetovolador.cpp \
    src/olagigante.cpp \
    src/arrecife.cpp \
    src/juego.cpp

HEADERS += \
    include/mainwindow.h \
    include/entidad.h \
    include/barco.h \
    include/capitanbarbarroja.h \
    include/obstaculo.h \
    include/item.h \
    include/corriente.h \
    include/nivel.h \
    include/nivel1archipielago.h \
    include/nivel2oscuridad.h \
    include/kraken.h \
    include/agentekraken.h \
    include/objetovolador.h \
    include/olagigante.h \
    include/arrecife.h \
    include/juego.h \
    include/tipos.h

INCLUDEPATH += include

RESOURCES += resources/resources.qrc
