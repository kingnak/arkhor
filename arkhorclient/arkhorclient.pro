QT += network

HEADERS += \
    connectionhandler.h \
    form.h \
    ahmaingui.h \
    ahboardscene.h \
    ahfielditem.h \
    itemstacker.h \
    ahgraphicsview.h \
    flowlayout.h \
    ahboardfillerhelper.h \
    movementchooser.h \
    focuschooser.h \
    optionchooser.h \
    dierollwidget.h \
    objectregistry.h \
    characterwidget.h \
    weaponchooser.h \
    monsterwidget.h \
    resourcepool.h \
    utils.h \
    registryobjectstackitem.h \
    diewidget.h

SOURCES += \
    connectionhandler.cpp \
    form.cpp \
    main.cpp \
    ahmaingui.cpp \
    ahboardscene.cpp \
    ahfielditem.cpp \
    itemstacker.cpp \
    ahgraphicsview.cpp \
    flowlayout.cpp \
    ahboardfillerhelper.cpp \
    movementchooser.cpp \
    focuschooser.cpp \
    optionchooser.cpp \
    dierollwidget.cpp \
    objectregistry.cpp \
    characterwidget.cpp \
    weaponchooser.cpp \
    monsterwidget.cpp \
    resourcepool.cpp \
    utils.cpp \
    registryobjectstackitem.cpp \
    diewidget.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../arkhorcommon/release/ -larkhorcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../arkhorcommon/debug/ -larkhorcommon
else:unix:!macx: LIBS += -L$$OUT_PWD/../arkhorcommon/ -larkhorcommon

INCLUDEPATH += $$PWD/../arkhorcommon
DEPENDPATH += $$PWD/../arkhorcommon

FORMS += \
    form.ui \
    ahmaingui.ui \
    movementchooser.ui \
    focuschooser.ui \
    optionchooser.ui \
    dierollwidget.ui \
    characterwidget.ui \
    weaponchooser.ui \
    monsterwidget.ui

RESOURCES += \
    client_resources.qrc
