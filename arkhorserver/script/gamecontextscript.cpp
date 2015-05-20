#include "gamecontextscript.h"
#include "characterscript.h"
#include "game/player.h"
#include "game/dietesthelper.h"
#include "monster.h"
#include "game/phases/fight/fightphase.h"
#include "gatescript.h"
#include <QDebug>

GameContextScript::GameContextScript(QObject *parent) :
    QObject(parent)
{
}

CharacterScript *GameContextScript::curCharacter()
{
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
