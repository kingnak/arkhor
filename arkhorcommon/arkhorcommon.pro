TEMPLATE = lib

QT -= gui
QT += network

DEFINES += ARKHOR_COMMON_DLL

HEADERS += \
    communication/message.h \
    communication/networkconnection.h \
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
    arkhamencounterdata.h \
    fielddata.h

SOURCES += \
    communication/message.cpp \
    communication/networkconnection.cpp \
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
    arkhamencounterdata.cpp \
    fielddata.cpp
