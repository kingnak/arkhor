#include "gamecontextscript.h"
#include "characterscript.h"
#include "game/player.h"
#include "game/dietesthelper.h"
#include "monster.h"
#include "game/phases/fight/fightphase.h"

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

/*
int GameContextScript::genericDieRollSum(int dieCount, QString desc)
{
    DieTestHelper::DieTestSpec spec = DieTestHelper::createGenericSummer(desc, dieCount);
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(gGame->context().player(), spec);
    return res.intResult;
    return 0;
}
*/

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
