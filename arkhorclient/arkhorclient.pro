QT += network widgets

unix:CONFIG(debug, debug|release): DEFINES += "_DEBUG=1"

HEADERS += \
    connectionhandler.h \
    ahboardscene.h \
    ahboardfillerhelper.h \
    objectregistry.h \
    resourcepool.h \
    utils.h \
    asyncobjectreceiver.h \
    gui/ahfielditem.h \
    gui/ahgraphicsview.h \
    gui/ahmaingui.h \
    gui/characterwidget.h \
    gui/dierollwidget.h \
    gui/diewidget.h \
    gui/flowlayout.h \
    gui/focuschooser.h \
    gui/gatedatawidget.h \
    gui/gatewidget.h \
    gui/itemstacker.h \
    gui/monsterwidget.h \
    gui/movementchooser.h \
    gui/optionchooser.h \
    gui/registryobjectstackitem.h \
    gui/startform.h \
    gui/weaponchooser.h \
    gui/gameobjectwidget.h \
    gui/mythosdatawidget.h \
    gui/objectselectorwidget.h \
    gui/choicewidget.h \
    gui/objectinfowidget.h \
    gui/paymentselectorwidget.h \
    gui/stringchoiceselectorwidget.h \
    gui/mythoscardwidget.h \
    gui/environmentwidget.h \
    gui/rumorwidget.h \
    gui/investigatorwidget.h \
    gui/ancientonewidget.h \
    gui/minsizetab.h

SOURCES += \
    connectionhandler.cpp \
    main.cpp \
    ahboardscene.cpp \
    ahboardfillerhelper.cpp \
    objectregistry.cpp \
    resourcepool.cpp \
    utils.cpp \
    asyncobjectreveiver.cpp \
    gui/ahfielditem.cpp \
    gui/ahgraphicsview.cpp \
    gui/ahmaingui.cpp \
    gui/characterwidget.cpp \
    gui/dierollwidget.cpp \
    gui/diewidget.cpp \
    gui/flowlayout.cpp \
    gui/focuschooser.cpp \
    gui/gatedatawidget.cpp \
    gui/gatewidget.cpp \
    gui/itemstacker.cpp \
    gui/monsterwidget.cpp \
    gui/movementchooser.cpp \
    gui/optionchooser.cpp \
    gui/registryobjectstackitem.cpp \
    gui/startform.cpp \
    gui/weaponchooser.cpp \
    gui/gameobjectwidget.cpp \
    gui/mythosdatawidget.cpp \
    gui/objectselectorwidget.cpp \
    gui/choicewidget.cpp \
    gui/objectinfowidget.cpp \
    gui/paymentselectorwidget.cpp \
    gui/stringchoiceselectorwidget.cpp \
    gui/mythoscardwidget.cpp \
    gui/environmentwidget.cpp \
    gui/rumorwidget.cpp \
    gui/investigatorwidget.cpp \
    gui/ancientonewidget.cpp \
    gui/minsizetab.cpp

FORMS += \
    gui/characterwidget.ui \
    gui/dierollwidget.ui \
    gui/focuschooser.ui \
    gui/gatedatawidget.ui \
    gui/monsterwidget.ui \
    gui/movementchooser.ui \
    gui/optionchooser.ui \
    gui/startform.ui \
    gui/weaponchooser.ui \
    gui/gameobjectwidget.ui \
    gui/mythosdatawidget.ui \
    gui/objectselectorwidget.ui \
    gui/choicewidget.ui \
    gui/mythoscardwidget.ui \
    gui/ancientonewidget.ui \
    gui/_ahmaingui.ui \
    gui/ahmaingui.ui

# Link to arkhorcommon
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../arkhorcommon/release/ -larkhorcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../arkhorcommon/debug/ -larkhorcommon
else:unix:!macx: LIBS += -L$$OUT_PWD/../arkhorcommon/ -larkhorcommon

INCLUDEPATH += $$PWD/../arkhorcommon
DEPENDPATH += $$PWD/../arkhorcommon

# Link to quazip
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../3rdparty/compiled/release/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../3rdparty/compiled/debug/ -lquazip
else:unix:!macx:CONFIG(release, release|debug): LIBS += -L$$PWD/../3rdparty/compiled/release/ -lquazip
else:unix:!macx:CONFIG(debug, release|debug): LIBS += -L$$PWD/../3rdparty/compiled/debug/ -lquazip

INCLUDEPATH += $$PWD/../3rdparty/compiled/include/
INCLUDEPATH += $$PWD/../3rdparty/compiled/include/zlib
# DEPENDPATH += $$PWD/../3rdparty/compiled/include/
