TEMPLATE = subdirs

#CONFIG(debug):DEFINES += "_DEBUG=1"

CONFIG += ordered

SUBDIRS += \
    arkhorcommon \
	arkhorscript \
    arkhorserver \
    arkhorclientstaticres \
	arkhorclient

#RESOURCES += \
#    arkhorclientstaticres/client_resources.qrc
