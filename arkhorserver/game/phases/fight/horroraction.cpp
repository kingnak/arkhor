#include "horroraction.h"
#include "game/game.h"
#include "character.h"
#include "game/player.h"
#include "game/dietesthelper.h"
#include "monster.h"
#include "fightphase.h"

HorrorAction::HorrorAction(FightPhase *fight)
    : m_fight(fight)
{
}


bool HorrorAction::execute()
{
    Monster *m = gGame->context().monster();
    Player *p = gGame->context().player();

    if (m->horrorDamage() > 0) {
        DieTestHelper::DieTestSpec test = DieTestHelper::createSkillTest(p->getCharacter(), AH::Skill_Will, m->horrorAdjustment());
        DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(p, test);
        if (res.boolResult) {
            m_fight->updatePhaseByResult(FightPhase::HorrorSuccess);
        } else {
            m_fight->updatePhaseByResult(FightPhase::HorrorFailed);
        }
    } else {
        m_fight->updatePhaseByResult(FightPhase::HorrorSuccess);
    }
    return true;
}
