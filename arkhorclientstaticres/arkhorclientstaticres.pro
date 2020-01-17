TEMPLATE = lib
CONFIG += static
CONFIG += resources_big

RESOURCES += \
	client_resources.qrc

CONFIG -= resources

buildrcc.target = .buildrcc
win32:CONFIG(debug, debug|release): buildrcc.commands = "$$PWD\\compile_rcc.bat" "$$PWD" "debug" "$$[QT_INSTALL_BINS]"
else:win32:CONFIG(release, debug|release):  buildrcc.commands = "$$PWD\\compile_rcc.bat" "$$PWD" "release" "$$[QT_INSTALL_BINS]"
else:unix:CONFIG(debug, debug|release): buildrcc.commands = "$$PWD/compile_rcc.sh" "$$PWD" "debug" "$$[QT_INSTALL_BINS]"
else:unix:CONFIG(release, debug|release):  buildrcc.commands = "$$PWD/compile_rcc.sh" "$$PWD" "release" "$$[QT_INSTALL_BINS]"

QMAKE_EXTRA_TARGETS += buildrcc
PRE_TARGETDEPS += .buildrcc

OTHER_FILES += \
	compile_rcc.bat \
    compile_rcc.sh
