QT += network

HEADERS += \
    connectionhandler.h \
    form.h \
    ahmaingui.h \
    ahboardscene.h \
    ahfielditem.h \
    itemstacker.h \
    ahgraphicsview.h

SOURCES += \
    connectionhandler.cpp \
    form.cpp \
    main.cpp \
    ahmaingui.cpp \
    ahboardscene.cpp \
    ahfielditem.cpp \
    itemstacker.cpp \
    ahgraphicsview.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../arkhorcommon/release/ -larkhorcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../arkhorcommon/debug/ -larkhorcommon
else:unix:!macx: LIBS += -L$$OUT_PWD/../arkhorcommon/ -larkhorcommon

INCLUDEPATH += $$PWD/../arkhorcommon
DEPENDPATH += $$PWD/../arkhorcommon

FORMS += \
    form.ui \
    ahmaingui.ui

RESOURCES += \
    client_resources.qrc
