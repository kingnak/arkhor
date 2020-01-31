#CONFIG += DEBUG_SCRIPT
#CONFIG += TEST_SCRIPT

CONFIG += console
CONFIG -= windows
QT += network script scripttools

DEBUG_SCRIPT|TEST_SCRIPT {
    QT += widgets
    DEFINES += DEBUG_SCRIPT_BUILD
    TEST_SCRIPT {
        DEFINES += TEST_SCRIPT_BUILD
    }
} else {
	QT -= gui widgets
}

unix:CONFIG(debug, debug|release): DEFINES += "_DEBUG=1"

HEADERS += \
    communication/clientconnection.h \
    communication/gameserver.h \
    communication/networkplayer.h \
    die/die.h \
    die/diefactory.h \
    die/diepool.h \
    die/dierollcountevaluator.h \
    die/dierollevaluator.h \
    die/dierollexecutor.h \
    die/dierollresult.h \
    die/dierollsumevaluator.h \
    die/dierolltest.h \
    game/actions/arkhamencounteryaction.h \
    game/actions/closegateaction.h \
    game/actions/entergateaction.h \
	game/actions/focusaction.h \
    game/actions/moveaction.h \
    game/actions/otherworldencounteryaction.h \
    game/actions/skipoption.h \
    game/actions/tradeaction.h \
    game/actions/untapaction.h \
    game/phases/arkhamencoutery.h \
    game/phases/chooseinvestigator.h \
    game/phases/fight/choosemonsteraction.h \
    game/phases/movement.h \
    game/phases/otherworldencountery.h \
    game/phases/upkeep.h \
    game/broadcastnotifier.h \
    game/game.h \
    game/gameaction.h \
    game/gameboard.h \
    game/gamecontext.h \
    game/gamefield.h \
    game/gameinteractor.h \
    game/gamenotifier.h \
    game/gameobject.h \
    game/gameoption.h \
    game/gamephase.h \
    game/investigator.h \
    game/movementpath.h \
    game/player.h \
    game/propertyvalue.h \
#    game/simplegameinteractor.h \
#    game/simplegamenotifier.h \
    script/characterscript.h \
    script/gameactionscript.h \
    script/gameobjectscript.h \
    script/gameoptionscript.h \
    script/gamescript.h \
    script/investigatorscript.h \
    attributeslider.h \
    character.h \
    deck.hpp \
    gate.h \
    monster.h \
    randomsource.h \
    game/propertymodifier.h \
    script/propertymodificationscript.h \
    game/arkhamencounter.h \
    script/arkhamencounterscript.h \
    game/gameregistry.h \
    game/dietesthelper.h \
    script/monsterscript.h \
    game/phases/fight/evadeaction.h \
    game/phases/fight/fightaction.h \
    game/phases/fight/fightphase.h \
    game/phases/fight/horroraction.h \
    game/phases/fight/chooseweaponsaction.h \
    game/phases/fight/attackaction.h \
    game/actions/chooseskillhelper.h \
    script/gamecontextscript.h \
    script/otherworldencounterscript.h \
    game/otherworldencounter.h \
    game/phases/mythos.h \
    game/mythoscard.h \
    script/mythoscardscript.h \
    game/actions/setoutaction.h \
    game/actions/undelayaction.h \
    game/actions/placeonfieldaction.h \
    script/gamefieldscript.h \
    script/gatescript.h \
    game/actions/dierollaction.h \
    game/drawcardhelper.h \
    game/choicehelper.h \
    game/actions/castspellaction.h \
    script/dynamicpropertyscript.h \
    ancientone.h \
    script/ancientonescript.h \
    game/phases/ancientoneattack.h \
    game/phases/attackancientonephase.h \
    game/monstermodifier.h \
    script/monstermodifierscript.h \
    communication/notificationformatter.h \
    game/actions/preventdamageaction.h \
    script/preventdamageoptionscript.h \
    scripttest/scripttestconfig.h \
    scripttest/scripttestconfigwidget.h \
    scripttest/scripttestdrawwidget.h \
    scripttest/scripttestdierollwidget.h

SOURCES += \
    communication/clientconnection.cpp \
    communication/gameserver.cpp \
    communication/networkplayer.cpp \
    die/die.cpp \
    die/diefactory.cpp \
    die/diepool.cpp \
    die/dierollcountevaluator.cpp \
    die/dierollevaluator.cpp \
    die/dierollexecutor.cpp \
    die/dierollresult.cpp \
    die/dierollsumevaluator.cpp \
    die/dierolltest.cpp \
    game/actions/arkhamencounteryaction.cpp \
    game/actions/closegateaction.cpp \
    game/actions/entergateaction.cpp \
	game/actions/focusaction.cpp \
    game/actions/moveaction.cpp \
    game/actions/otherworldencounteryaction.cpp \
    game/actions/tradeaction.cpp \
    game/actions/untapaction.cpp \
    game/phases/arkhamencoutery.cpp \
    game/phases/chooseinvestigator.cpp \
    game/phases/fight/choosemonsteraction.cpp \
    game/phases/movement.cpp \
    game/phases/otherworldencountery.cpp \
    game/phases/upkeep.cpp \
    game/broadcastnotifier.cpp \
    game/game.cpp \
    game/gameaction.cpp \
    game/gameboard.cpp \
    game/gamecontext.cpp \
    game/gamefield.cpp \
    game/gameinteractor.cpp \
    game/gamenotifier.cpp \
    game/gameobject.cpp \
    game/gameoption.cpp \
    game/gamephase.cpp \
    game/investigator.cpp \
    game/movementpath.cpp \
    game/player.cpp \
    game/propertyvalue.cpp \
#    game/simplegameinteractor.cpp \
#    game/simplegamenotifier.cpp \
    script/characterscript.cpp \
    script/gameactionscript.cpp \
    script/gameobjectscript.cpp \
    script/gameoptionscript.cpp \
    script/gamescript.cpp \
    script/investigatorscript.cpp \
    attributeslider.cpp \
    character.cpp \
    gate.cpp \
    main.cpp \
    monster.cpp \
    randomsource.cpp \
    game/propertymodifier.cpp \
    script/propertymodificationscript.cpp \
    game/arkhamencounter.cpp \
    script/arkhamencounterscript.cpp \
    game/gameregistry.cpp \
    game/dietesthelper.cpp \
    script/monsterscript.cpp \
    game/phases/fight/evadeaction.cpp \
    game/phases/fight/fightaction.cpp \
    game/phases/fight/fightphase.cpp \
    game/phases/fight/horroraction.cpp \
    game/phases/fight/chooseweaponsaction.cpp \
    game/phases/fight/attackaction.cpp \
    game/actions/chooseskillhelper.cpp \
    script/gamecontextscript.cpp \
    game/otherworldencounter.cpp \
    script/otherworldencounterscript.cpp \
    game/phases/mythos.cpp \
    game/mythoscard.cpp \
    script/mythoscardscript.cpp \
    game/actions/setoutaction.cpp \
    game/actions/undelayaction.cpp \
    game/actions/placeonfieldaction.cpp \
    script/gamefieldscript.cpp \
    script/gatescript.cpp \
    game/actions/dierollaction.cpp \
    game/drawcardhelper.cpp \
    game/choicehelper.cpp \
    game/actions/castspellaction.cpp \
    script/dynamicpropertyscript.cpp \
    ancientone.cpp \
    script/ancientonescript.cpp \
    game/phases/ancientoneattack.cpp \
    game/phases/attackancientonephase.cpp \
    game/monstermodifier.cpp \
    script/monstermodifierscript.cpp \
    communication/notificationformatter.cpp \
    game/actions/preventdamageaction.cpp \
    script/preventdamageoptionscript.cpp \
    scripttest/scripttestconfig.cpp \
    scripttest/scripttestconfigwidget.cpp \
    scripttest/scripttestdrawwidget.cpp \
    scripttest/scripttestdierollwidget.cpp

OTHER_FILES += \
    data/field/arkham/frenchHill/theWitchhouse.js \
    data/field/arkham/miskatonicU/administration.js \
    data/objects/common/weapons.js \
    data/monsters/monsters.js \
    data/field/otherworld/abyss.js \
    data/mythos/mythos.js \
    data/monsters/unfinished.js \
    data/objects/common/other.js \
    data/objects/common/unfinished.js \
    data/objects/skills/skills.js \
    data/objects/skills/unfinished.js \
    data/objects/allies/allies.js \
    data/objects/allies/unfinished.js \
    data/objects/spells/misc.js \
    data/objects/spells/unfinished.js \
    data/objects/spells/weapons.js \
    data/objects/unique/misc.js \
    data/objects/unique/tomes.js \
    data/objects/unique/unfinished.js \
    data/objects/unique/weapons.js \
    data/objects/test.js \
    data/mythos/rumor.js \
    data/objects/blessing_curse.ahs \
    data/mythos/rumor.ahs \
    data/mythos/env_urban.ahs \
    data/ancientone/azathoth.ahs \
    data/ancientone/hastur.ahs \
    data/characters/sister_mary.ahs \
    data/characters/jenny_barnes.ahs \
    data/characters/amanda_sharpe.ahs \
    data/characters/ashcan_pete.ahs \
    data/field/arkham/downtown/arkhamAsylum.ahs \
    data/field/arkham/downtown/bankOfArkham.ahs \
    data/field/arkham/downtown/independenceSquare.ahs \
    data/field/arkham/easttown/hibbsRoadhouse.ahs \
    data/1global/common.ahs \
    data/field/arkham/easttown/policeStation.ahs \
    data/field/arkham/easttown/velmasDiner.ahs \
    data/field/arkham/frenchHill/silverTwilightLodge.ahs \
    data/field/arkham/frenchHill/theWitchhouse.ahs \
    data/field/arkham/merchantDist/riverDocks.ahs \
    data/field/arkham/merchantDist/theUnnamable.ahs \
    data/field/arkham/merchantDist/unvisitedIsle.ahs \
    data/field/arkham/miskatonicU/administration.ahs \
    data/field/arkham/miskatonicU/library.ahs \
    data/field/arkham/miskatonicU/scienceBuilding.ahs \
    data/field/arkham/northside/curiosityShoppe.ahs \
    data/field/arkham/northside/newspaper.ahs \
    data/field/arkham/northside/trainstation.ahs \
    data/field/arkham/rivertown/blackCave.ahs \
    data/field/arkham/rivertown/generalStore.ahs \
    data/field/arkham/rivertown/graveyard.ahs \
    data/field/arkham/southside/historicalSociety.ahs \
    data/field/arkham/southside/masBoardingHouse.ahs \
    data/field/arkham/uptown/stMarysHospital.ahs \
    data/field/arkham/uptown/woods.ahs \
    data/field/arkham/uptown/yeOldeMagickShoppe.ahs \
    data/field/arkham/southside/southChurch.ahs \
    data/monsters/monsters.ahs \
    data/objects/allies/allies.ahs \
    data/objects/common/other.ahs

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../arkhorcommon/release/ -larkhorcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../arkhorcommon/debug/ -larkhorcommon
else:unix:!macx: LIBS += -L$$OUT_PWD/../arkhorcommon/ -larkhorcommon

INCLUDEPATH += $$PWD/../arkhorcommon
DEPENDPATH += $$PWD/../arkhorcommon

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../arkhorscript/release/ -larkhorscript
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../arkhorscript/debug/ -larkhorscript
else:unix: LIBS += -L$$OUT_PWD/../arkhorscript/ -larkhorscript

INCLUDEPATH += $$PWD/../arkhorscript
DEPENDPATH += $$PWD/../arkhorscript

# Link to quazip
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../3rdparty/compiled/release/msvc2017_64 -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../3rdparty/compiled/debug/msvc2017_64 -lquazip
else:unix:!macx:CONFIG(release, release|debug): LIBS += -L$$PWD/../3rdparty/compiled/release/linux -lquazip
else:unix:!macx:CONFIG(debug, release|debug): LIBS += -L$$PWD/../3rdparty/compiled/debug/linux -lquazip

INCLUDEPATH += $$PWD/../3rdparty/compiled/include/
INCLUDEPATH += $$PWD/../3rdparty/compiled/include/zlib
# DEPENDPATH += $$PWD/../3rdparty/compiled/include/

DISTFILES += \
    data/ancientone/cthulhu.ahs \
    data/characters/bob_jenkins.ahs \
    data/characters/carolyn_fern.ahs \
    data/characters/darrell_simmons.ahs \
    data/characters/dexter_drake.ahs \
    data/characters/gloria_goldberg.ahs \
    data/characters/harvey_walters.ahs \
    data/characters/joe_diamond.ahs \
    data/characters/kate_winthrop.ahs \
    data/characters/mandy_thompson.ahs \
    data/characters/michael_mcglen.ahs \
    data/characters/monterey_jack.ahs \
    data/characters/vincent_lee.ahs \
    data/mythos/ru_disturbing_the_dead.ahs \
    data/mythos/ru_southside_strangler.ahs \
    data/mythos/test.ahs \
    data/objects/common/weapons.ahs \
    data/objects/skills/skills.ahs \
    data/objects/special/special.ahs \
    data/objects/spells/misc.ahs \
    data/objects/spells/weapons.ahs \
    data/objects/unique/misc.ahs \
    data/objects/unique/tomes.ahs \
    data/objects/unique/weapons.ahs \
    data/ancientone/ithaqua.ahs \
    data/ancientone/nyarlathotep.ahs \
    data/ancientone/shub_niggurath.ahs \
    data/ancientone/yig.ahs \
    data/ancientone/yog_sothoth.ahs \
    data/mythos/env_mystic.ahs \
    data/mythos/headlines.ahs \
    data/mythos/env_weather.ahs \
    data/1global/utility.js
