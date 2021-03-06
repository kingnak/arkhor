#include "attackancientonephase.h"
#include "game/dietesthelper.h"
#include "ancientone.h"
#include "game/player.h"
#include "character.h"

bool AttackAncientOneAction::execute()
{
    if (gGame->ancientOne()->isDefeated()) return true;

    AncientOne *ao = gGame->ancientOne();
    Player *p = gGame->context().player();

    gGame->notifier()->actionStart(this);

    auto base = this->getAttackModifications(p->getCharacter());
    ModifiedPropertyValue pool = base.finalPropery();

    ModifiedPropertyValue clueBurnMods = gGame->context().getCurCharacterClueBurn(AH::Skill_Combat);

    DieTestHelper::DieTestSpec test = DieTestHelper::createClueBurnCounter("Attack " + ao->name(), ao->id(), p->getCharacter(), pool, clueBurnMods, ao->combatAdjustment());
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(p, test);

    ao->damage(res.intResult);

    // TODO: Call post-use methods for equipped objects ==> Needed?

    // Remove single use objects
    discardAfterAttack(base.weaponsGeneral);
    discardAfterAttack(base.weaponsPhysical);
    discardAfterAttack(base.weaponsMagical);

    gGame->notifier()->actionFinish(this);

    return true;
}

PropertyModificationList AttackAncientOneAction::getMonsterModifications(Character *c) const
{
    Q_UNUSED(c)
    return gGame->context().ancientOne()->getCombatModifications();
}

///////////////////////

AttackAncientOnePhase::AttackAncientOnePhase(Game *g)
    : GamePhase(g)
{
    m_choose = new ChooseWeaponsOption(nullptr);
    m_attack = new AttackAncientOneOption;
}

AttackAncientOnePhase::~AttackAncientOnePhase()
{
    delete m_choose;
    delete m_attack;
}

QList<GameOption *> AttackAncientOnePhase::getPhaseOptions()
{
    if (gGame->ancientOne()->isDefeated()) {
        return QList<GameOption *>() << getSkipOption();
    }
    return QList<GameOption *>() << m_choose << m_attack;
}

