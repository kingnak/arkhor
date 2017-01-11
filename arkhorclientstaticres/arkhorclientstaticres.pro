TEMPLATE = lib
CONFIG += static
CONFIG += resources_big

RESOURCES += \
	client_resources.qrc

CONFIG -= resources

buildrcc.target = .buildrcc
win32:CONFIG(debug, debug|release): buildrcc.commands = "$$PWD\\compile_rcc.bat" "$$PWD" "debug"
else:win32:CONFIG(release, debug|release):  buildrcc.commands = "$$PWD\\compile_rcc.bat" "$$PWD" "release"
else:unix:CONFIG(debug, debug|release): buildrcc.commands = "$$PWD/compile_rcc.sh" "$$PWD" "debug"
else:unix:CONFIG(release, debug|release):  buildrcc.commands = "$$PWD/compile_rcc.sh" "$$PWD" "release"

QMAKE_EXTRA_TARGETS += buildrcc
PRE_TARGETDEPS += .buildrcc

OTHER_FILES += \
	compile_rcc.bat \
    compile_rcc.sh
