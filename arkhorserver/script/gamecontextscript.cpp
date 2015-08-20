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
    foreach (Character *c, gGame->registry()->allCharacters()) {
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

int GameContextScript::genericDieRollSum(QString desc, int dieCount)
{
    DieTestHelper::DieTestSpec spec = DieTestHelper::createGenericSummer(desc, dieCount);
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(gGame->context().player(), spec);
    return res.intResult;
}

bool GameContextScript::genericDieRollTest(QString desc, int dieCount, int target)
{
    int ct = genericDieRollCount(desc, dieCount);
    return ct >= target;
}

int GameContextScript::genericDieRollCount(QString desc, int dieCount)
{
    QList<quint32> successes;
    for (int i = gGame->context().getCurCharacterProperty(PropertyValue::Prop_MinSuccessDieRoll).finalVal(); i <= 6; ++i) {
        successes << i;
    }
    DieTestHelper::DieTestSpec spec = DieTestHelper::createGenericCounter(desc, dieCount, successes);
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(gGame->context().player(), spec);
    return res.intResult;
}

bool GameContextScript::skillTest(QString desc, int skill, int adjust, int target)
{
    AH::Skill sk = static_cast<AH::Skill> (skill);
    DieTestHelper::DieTestSpec spec = DieTestHelper::createSkillTest(desc, gGame->context().player()->getCharacter(), sk, adjust, target);
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(gGame->context().player(), spec);
    return res.boolResult;
}

int GameContextScript::dieRollSkillCount(QString desc, int skill, int adjust)
{
    AH::Skill sk = static_cast<AH::Skill> (skill);
    DieTestHelper::DieTestSpec spec = DieTestHelper::createSkillCounter(desc, gGame->context().player()->getCharacter(), sk, adjust);
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(gGame->context().player(), spec);
    return res.intResult;
}

QString GameContextScript::selectChoice(QString desc, QList<AH::Common::ChoiceData::OptionData> options)
{
    return selectChoice(desc, options, false);
}

QString GameContextScript::selectChoice(QString desc, QList<AH::Common::ChoiceData::OptionData> options, bool canCancel)
{
    AH::Common::ChoiceData ch;
    ch.setSelectStrings(options);
    ch.setDescription(desc);
    ch.setCanCancel(canCancel);
    AH::Common::ChoiceResponseData res = gGame->context().player()->offerChoice(ch);
    return res.toString();
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

GameObjectScript *GameContextScript::drawObject(qint32 type)
{
    return drawObject(type, "Select Object");
}

GameObjectScript *GameContextScript::drawObject(qint32 type, QString desc)
{
    AH::GameObjectType t = static_cast<AH::GameObjectType> (type);

    ModifiedPropertyValue val = gGame->context().getCurCharacterDrawObject(t);
    int ct = val.finalVal();

    DrawCardHelper hlp;
    QList<GameObject *> objs = hlp.drawObjects(gGame->context().player(), desc, t, ct, 1, 1);

    GameObject *o = objs.value(0);
    if (!o) return NULL;
    GameObjectScript *os = dynamic_cast<GameObjectScript *> (o);
    if (!os) {
        gGame->returnObject(o);
        qWarning() << "Drawn object was not a script object";
    }
    return os;
}

QList<GameObjectScript *> GameContextScript::drawMultipleObjects(qint32 type, QString desc, int count, int min, int max)
{
    AH::GameObjectType t = static_cast<AH::GameObjectType> (type);

    ModifiedPropertyValue val = gGame->context().getCurCharacterDrawObject(t);
    int ct = val.modifiers().apply(count);

    DrawCardHelper hlp;
    QList<GameObject *> objs = hlp.drawObjects(gGame->context().player(), desc, t, ct, min, max);

    QList<GameObjectScript *> ret;
    foreach (GameObject *o, objs) {
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

QList<GameObjectScript *> GameContextScript::drawMixedObjects(QString desc, QList<AH::ObjectTypeCount> types, int min, int max)
{
    // TODO: Handle modifiers?

    DrawCardHelper hlp;
    QList<GameObject *> objs = hlp.drawMixedObjects(gGame->context().player(), desc, types, min, max);

    QList<GameObjectScript *> ret;
    foreach (GameObject *o, objs) {
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
