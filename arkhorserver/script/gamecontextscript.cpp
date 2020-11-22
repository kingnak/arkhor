#include "gamecontextscript.h"
#include "characterscript.h"
#include "game/player.h"
#include "game/dietesthelper.h"
#include "monster.h"
#include "game/phases/fight/fightphase.h"
#include "gatescript.h"
#include "gameobjectscript.h"
#include "game/drawcardhelper.h"
#include "gamescript.h"
#include "mythoscardscript.h"
#include "monsterscript.h"
#include "ancientonescript.h"
#include <QDebug>

GameContextScript::GameContextScript(QObject *parent) :
    QObject(parent)
{
}

CharacterScript *GameContextScript::curCharacter()
{
#ifdef DEBUG_SCRIPT_BUILD
    if (!gGame->context().player()) return NULL;
#endif
    Character *c = gGame->context().player()->getCharacter();
    CharacterScript *cs = dynamic_cast<CharacterScript *> (c);
    return cs;
}

QObjectList GameContextScript::getAllCharacters()
{
    QObjectList ret;
    for (auto c : gGame->registry()->allCharacters()) {
        CharacterScript *cs = dynamic_cast<CharacterScript *> (c);
        if (cs) {
            ret << cs;
        } else {
            qCritical() << "Non-Script Character detected in GameContextScript::getAllCharacters";
        }
    }

    return ret;
}

GateScript *GameContextScript::curGate()
{
    Gate *g = gGame->context().gate();
    if (g) {
        GateScript *gs = new GateScript(g);
        return gs;
    }
    return NULL;
}

int GameContextScript::getTerrorLevel()
{
    return gGame->terrorLevel();
}

MythosCardScript *GameContextScript::activeRumor()
{
    MythosCard *r = gGame->rumor();
    MythosCardScript *rs = dynamic_cast<MythosCardScript *> (r);
    return rs;
}

MonsterScript *GameContextScript::curMonster()
{
    Monster *m = gGame->context().monster();
    MonsterScript *ms = dynamic_cast<MonsterScript *> (m);
    return ms;
}

AncientOneScript *GameContextScript::curAncientOne()
{
    AncientOne *ao = gGame->context().ancientOne();
    AncientOneScript *aos = dynamic_cast<AncientOneScript *> (ao);
    return aos;
}

int GameContextScript::genericDieRollSum(QString desc, QString sourceId, int dieCount)
{
    return curCharacter()->genericDieRollSum(desc, sourceId, dieCount);
}

bool GameContextScript::genericDieRollTest(QString desc, QString sourceId, int dieCount, int target)
{
    return curCharacter()->genericDieRollTest(desc, sourceId, dieCount, target);
}

int GameContextScript::genericDieRollCount(QString desc, QString sourceId, int dieCount)
{
    return curCharacter()->genericDieRollCount(desc, sourceId, dieCount);
}

int GameContextScript::genericDieRollCountSuccess(QString desc, QString sourceId, int dieCount, QScriptValue successes)
{
    return curCharacter()->genericDieRollCountSuccess(desc, sourceId, dieCount, successes);
}

bool GameContextScript::skillTest(QString desc, QString sourceId, AH::Skill skill, int adjust, int target)
{
    return curCharacter()->skillTest(desc, sourceId, skill, adjust, target);
}

int GameContextScript::dieRollSkillCount(QString desc, QString sourceId, AH::Skill skill, int adjust)
{
    return curCharacter()->dieRollSkillCount(desc, sourceId, skill, adjust);
}

void GameContextScript::increaseTerrorLevel(int amount)
{
    gGame->increaseTerrorLevel(amount);
}

QString GameContextScript::selectChoice(QString desc, const QString &sourceId, QList<AH::Common::ChoiceData::OptionData> options)
{
    return selectChoice(desc, sourceId, options, false);
}

QString GameContextScript::selectChoice(QString desc, const QString &sourceId, QList<AH::Common::ChoiceData::OptionData> options, bool canCancel)
{
    return gGame->context().selectChoice(desc, sourceId, options, canCancel);
}

bool GameContextScript::spontaneousMonsterFight()
{
    Monster *m = gGame->drawMonster();
    m->setSpontaneous(true);

    FightPhase fight;
    fight.handleFight(QList<Monster*>() << m);

    if (fight.getOutcome() == FightPhase::EndFailed) {
        return false;
    }
    return true;
}

bool GameContextScript::createGateAtCharacterField()
{
    return gGame->createGate(gGame->context().player()->getCharacter()->field());
}

QScriptValue GameContextScript::allFields(int type)
{
    auto fields = gGame->board()->fields(static_cast<AH::Common::FieldData::FieldType> (type));
    QList<GameFieldScript*> ret;
    for (auto f : fields) {
        ret << gGameScript->getField(f);
    }
    return GameScript::makeArray(ret);
}

GameObjectScript *GameContextScript::drawObject(AH::GameObjectType type)
{
    return drawObject(type, "Select Object", QString::null);
}

GameObjectScript *GameContextScript::drawObject(AH::GameObjectType type, QString desc, QString sourceId)
{
    ModifiedPropertyValue val = gGame->context().getCurCharacterDrawObject(type);
    int ct = val.finalVal();

    DrawCardHelper hlp;
    QList<GameObject *> objs = hlp.drawObjects(gGame->context().player(), desc, type, ct, 1, 1, sourceId.isNull(), sourceId);

    GameObject *o = objs.value(0);
    if (!o) return NULL;
    GameObjectScript *os = dynamic_cast<GameObjectScript *> (o);
    if (!os) {
        gGame->returnObject(o);
        qWarning() << "Drawn object was not a script object";
    }
    return os;
}

QList<GameObjectScript *> GameContextScript::selectObjects(QList<GameObjectScript *> objects, QString desc, const QString &sourceId, int min, int max)
{
    return curCharacter()->selectObjects(objects, desc, sourceId, min, max);
}

QList<GameObjectScript *> GameContextScript::drawMultipleObjects(AH::GameObjectType type, QString desc, const QString &sourceId, int count, int min, int max)
{
    ModifiedPropertyValue val = gGame->context().getCurCharacterDrawObject(type);
    int ct = val.modifiers().apply(count);

    DrawCardHelper hlp;
    QList<GameObject *> objs = hlp.drawObjects(gGame->context().player(), desc, type, ct, min, max, sourceId.isNull(), sourceId);

    QList<GameObjectScript *> ret;
    for (auto o : objs) {
        GameObjectScript *os = dynamic_cast<GameObjectScript *> (o);
        if (!os) {
            gGame->returnObject(o);
            qWarning() << "Drawn object was not a script object";
        } else {
            ret << os;
        }
    }
    return ret;
}

QList<GameObjectScript *> GameContextScript::drawMixedObjects(QString desc, const QString &sourceId, QList<AH::ObjectTypeCount> types, int min, int max)
{
    // TODO: Handle modifiers?

    DrawCardHelper hlp;
    QList<GameObject *> objs = hlp.drawMixedObjects(gGame->context().player(), desc, types, min, max, sourceId.isNull(), sourceId);

    QList<GameObjectScript *> ret;
    for (auto o : objs) {
        GameObjectScript *os = dynamic_cast<GameObjectScript *> (o);
        if (!os) {
            gGame->returnObject(o);
            qWarning() << "Drawn object was not a script object";
        } else {
            ret << os;
        }
    }
    return ret;
}
