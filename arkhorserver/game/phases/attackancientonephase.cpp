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

    // Get base damage + weapon damages
    ModifiedPropertyValue base = gGame->context().getCurCharacterSkill(AH::Skill_Combat);
    PropertyModificationList gen = p->getCharacter()->getPropertyModifiers().filtered(PropertyValue::Damage_General);
    PropertyModificationList mag = p->getCharacter()->getPropertyModifiers().filtered(PropertyValue::Damage_Magical);
    PropertyModificationList phy = p->getCharacter()->getPropertyModifiers().filtered(PropertyValue::Damage_Physical);

    // Get only equipped
    filterEquipped(gen);
    filterEquipped(mag);
    filterEquipped(phy);

    // Possible for AO?
    //MonsterData::MonsterAttributes ignoredAttributes = gGame->context().getCurCharacterIgnoredMonsterAttributes();

    //ao->defenses();
    PropertyModificationList monsterMods = ao->getCombatModifications(/*ignoredAttributes*/);
    phy += monsterMods.filtered(PropertyValue::Damage_Physical);
    mag += monsterMods.filtered(PropertyValue::Damage_Magical);

    int dmgBase = base.finalVal();
    int dmgGen = gen.apply(0);
    int dmgMag = mag.apply(0);
    int dmgPhy = phy.apply(0);

    // Calculate final pool
    int dmgTotal = dmgBase + dmgGen + dmgMag + dmgPhy;
    ModifiedPropertyValue pool(PropertyValue(PropertyValue::Skill_Combat, dmgBase), dmgTotal, base.modifiers()+gen+mag+phy);

    ModifiedPropertyValue clueBurnMods = gGame->context().getCurCharacterClueBurn(AH::Skill_Combat);

    DieTestHelper::DieTestSpec test = DieTestHelper::createClueBurnCounter("Attack " + ao->name(), p->getCharacter(), pool, clueBurnMods, ao->combatAdjustment());
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(p, test);

    ao->damage(res.intResult);

    // TODO: Call post-use methods for equipped objects ==> Needed?

    // Remove single use objects
    discardAfterAttack(gen);
    discardAfterAttack(mag);
    discardAfterAttack(phy);

    gGame->notifier()->actionFinish(this);

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
    if (gGame->ancientOne()->isDefeated()) {
        return QList<GameOption *>() << getSkipOption();
    }
    return QList<GameOption *>() << m_choose << m_attack;
}

