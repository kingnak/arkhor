#include "attackancientonephase.h"
#include "game/dietesthelper.h"
#include "ancientone.h"
#include "game/player.h"

bool AttackAncientOneAction::execute()
{
    DieTestHelper::DieTestSpec spec = DieTestHelper::createSkillCounter("Attack " + gGame->ancientOne()->name(), gGame->context().player()->getCharacter(), AH::Skill_Combat, gGame->ancientOne()->combatAdjustment());
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(gGame->context().player(), spec);
    gGame->ancientOne()->damage(res.intResult);

    return true;
}

///////////////////////

AttackAncientOnePhase::AttackAncientOnePhase(Game *g)
    : GamePhase(g)
{
    m_choose = new ChooseWeaponsOption(NULL);
    m_attack = new AttackAncientOneOption;
}

AttackAncientOnePhase::~AttackAncientOnePhase()
{
    delete m_choose;
    delete m_attack;
}

QList<GameOption *> AttackAncientOnePhase::getPhaseOptions()
{
    return QList<GameOption *>() << m_choose << m_attack;
}

