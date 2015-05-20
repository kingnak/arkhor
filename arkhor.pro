TEMPLATE = subdirs

CONFIG(debug):DEFINES += "_DEBUG=1"

SUBDIRS += \
    arkhorcommon \
    arkhorserver \
    arkhorclient

RESOURCES += \
    arkhorclient/gui/client_resources.qrc \
    arkhorclient/gui/client_resources.qrc
