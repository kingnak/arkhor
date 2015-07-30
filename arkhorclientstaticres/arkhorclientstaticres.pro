TEMPLATE = lib


DEFINES += ARKHOR_CLIENTSTATICRES_DLL
QMAKE_RESOURCE_FLAGS += -threshold 0 -compress 9

RESOURCES += \
    client_resources.qrc

HEADERS += \
    arkhorclientstaticres_global.h \
    arkhamhorrorstaticresources.h

SOURCES += \
    arkhamhorrorstaticresources.cpp
