#-------------------------------------------------
#
# Project created by QtCreator 2015-07-31T20:54:26
#
#-------------------------------------------------

QT       -= gui

VERSION = 0.8.0.0

TARGET = arkhorscript
TEMPLATE = lib
VERSION = 0.8.1.0
CONFIG += skip_target_version_ext

DEFINES += ARKHORSCRIPT_LIBRARY

SOURCES += arkhorscriptparser.cpp \
    arkhorscriptlexer.cpp \
    arkhorscriptgenerator.cpp \
    classgenerator.cpp \
    commonitemgenerator.cpp \
    objectgenerator.cpp \
    skillgenerator.cpp \
    uniqueitemgenerator.cpp \
    spellgenerator.cpp \
    allygenerator.cpp \
    monstergenerator.cpp \
    specialobjectgenerator.cpp \
    optiongenerator.cpp \
    actiongenerator.cpp \
    quickoptiongenerator.cpp \
    fieldoptiongenerator.cpp \
    arkhamencountergenerator.cpp \
    otherworldencountergenerator.cpp \
    mythosgenerator.cpp \
    investigatorgenerator.cpp \
    ancientonegenerator.cpp \
    blessingcursegenerator.cpp \
    specialabilitygenerator.cpp

HEADERS += arkhorscriptparser.h\
        arkhorscript_global.h \
    arkhorscriptlexer.h \
    arkhorscriptgenerator.h \
    classgenerator.h \
    commonitemgenerator.h \
    objectgenerator.h \
    skillgenerator.h \
    uniqueitemgenerator.h \
    spellgenerator.h \
    allygenerator.h \
    monstergenerator.h \
    specialobjectgenerator.h \
    optiongenerator.h \
    actiongenerator.h \
    quickoptiongenerator.h \
    fieldoptiongenerator.h \
    arkhamencountergenerator.h \
    otherworldencountergenerator.h \
    mythosgenerator.h \
    investigatorgenerator.h \
    ancientonegenerator.h \
    blessingcursegenerator.h \
    specialabilitygenerator.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
