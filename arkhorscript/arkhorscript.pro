#-------------------------------------------------
#
# Project created by QtCreator 2015-07-31T20:54:26
#
#-------------------------------------------------

QT       -= gui

TARGET = arkhorscript
TEMPLATE = lib

DEFINES += ARKHORSCRIPT_LIBRARY

SOURCES += arkhorscriptparser.cpp \
    arkhorscriptlexer.cpp \
    arkhorscriptgenerator.cpp \
    classgenerator.cpp \
    commonitemgenerator.cpp \
    objectgenerator.cpp \
    skillgenerator.cpp \
    uniqueitemgenerator.cpp \
    spellgenerator.cpp

HEADERS += arkhorscriptparser.h\
        arkhorscript_global.h \
    arkhorscriptlexer.h \
    arkhorscriptgenerator.h \
    classgenerator.h \
    commonitemgenerator.h \
    objectgenerator.h \
    skillgenerator.h \
    uniqueitemgenerator.h \
    spellgenerator.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
