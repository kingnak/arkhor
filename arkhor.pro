TEMPLATE = subdirs

CONFIG(debug):DEFINES += "_DEBUG=1"

SUBDIRS += \
    arkhorcommon \
    arkhorserver \
    arkhorclient
