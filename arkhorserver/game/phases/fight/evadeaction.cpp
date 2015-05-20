#include "evadeaction.h"
#include "game/game.h"
#include "character.h"
#include "game/player.h"
#include "game/dietesthelper.h"
#include "monster.h"
#include "fightphase.h"

EvadeAction::EvadeAction(FightPhase *fight)
    :  m_hasFailedAttempt(false), m_fight(fight)
{
    m_name = "Evade";
}

bool EvadeAction::execute()
{
    Monster *m = gGame->context().monster();
    Player *p = gGame->context().player();

    DieTestHelper::DieTestSpec test = DieTestHelper::createSkillTest(p->getCharacter(), AH::Skill_Evade, m->awareness());
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(p, test);
    if (res.boolResult) {
        m_fight->updatePhaseByResult(FightPhase::CharacterFlown);
    } else {
        m_fight->updatePhaseByResult(FightPhase::EvadeFailed);
    }
    return true;
}

bool EvadeOption::isAvailable()
{
    Monster *m = gGame->context().monster();
    if (m->attributes().testFlag(AH::Common::MonsterData::Ambush)) {
        // Cannot evade
        return false;
    }
    return true;
}
