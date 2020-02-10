TEMPLATE = lib

QT -= gui
QT += network

DEFINES += ARKHOR_COMMON_DLL

HEADERS += \
    communication/message.h \
    communication/networkconnection.h \
    gamestatechangedata.h \
    tradedata.h \
    utils/cleanupthread.h \
    ahglobal.h \
    arkhorcommonglobal.h \
    characterdata.h \
    cost.h \
    gameactiondata.h \
    gamefielddata.h \
    gameobjectdata.h \
    gameoptiondata.h \
    investigatordata.h \
    playerdata.h \
    serializer.hpp \
    propertyvaluedata.h \
    fielddata.h \
    attributesliderdata.h \
    diedata.h \
    objectdata.h \
    monsterdata.h \
    encounterdata.h \
    mythosdata.h \
    gatedata.h \
    choicedata.h \
    dynamicpropertydata.h \
    ancientonedata.h \
    gamesettingdata.h

SOURCES += \
    communication/message.cpp \
    communication/networkconnection.cpp \
    gamestatechangedata.cpp \
    tradedata.cpp \
    utils/cleanupthread.cpp \
    ahglobal.cpp \
    characterdata.cpp \
    cost.cpp \
    gameactiondata.cpp \
    gamefielddata.cpp \
    gameobjectdata.cpp \
    gameoptiondata.cpp \
    investigatordata.cpp \
    playerdata.cpp \
    propertyvaluedata.cpp \
    fielddata.cpp \
    attributesliderdata.cpp \
    diedata.cpp \
    objectdata.cpp \
    monsterdata.cpp \
    encounterdata.cpp \
    mythosdata.cpp \
    gatedata.cpp \
    choicedata.cpp \
    dynamicpropertydata.cpp \
    ancientonedata.cpp \
    gamesettingdata.cpp
