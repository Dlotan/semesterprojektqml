TEMPLATE = app

QT += qml quick widgets
QT += sql
CONFIG += c++11
SOURCES += main.cpp \
    dicemaster.cpp \
    randomrangeclass.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = qml

# Subincludes.
include(generator/generator.pri)
include(database/database.pri)
# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    dicemaster.h \
    randomrangeclass.h
