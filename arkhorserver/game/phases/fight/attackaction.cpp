#include "attackaction.h"
#include "game/gameobject.h"
#include "game/game.h"
#include "game/player.h"
#include "game/dietesthelper.h"
#include "character.h"
#include "monster.h"
#include "fightphase.h"

AttackAction::AttackAction(FightPhase *fight)
    : m_fight(fight)
{
}

bool AttackAction::execute()
{
    Monster *m = gGame->context().monster();
    Player *p = gGame->context().player();

    // Get base damage + weapon damages
    ModifiedPropertyValue base = gGame->context().getCurCharacterSkill(AH::Skill_Combat);
    PropertyModificationList gen = p->getCharacter()->getPropertyModifiers().filtered(PropertyValue::Damage_General);
    PropertyModificationList mag = p->getCharacter()->getPropertyModifiers().filtered(PropertyValue::Damage_Magical);
    PropertyModificationList phy = p->getCharacter()->getPropertyModifiers().filtered(PropertyValue::Damage_Physical);

    // Get only equipped
    filterEquipped(gen);
    filterEquipped(mag);
    filterEquipped(phy);

    int dmgBase = base.finalVal();
    int dmgGen = gen.apply(0);
    int dmgMag = mag.apply(0);
    int dmgPhy = phy.apply(0);

    // Apply monster modifiers
    if (m->attributes().testFlag(AH::Common::MonsterData::PhysicalImmunity)) {
        dmgPhy = 0;
    }
    if (m->attributes().testFlag(AH::Common::MonsterData::PhysicalResistance)) {
        // rounded up half
        dmgPhy = (dmgPhy+1) / 2;
    }
    if (m->attributes().testFlag(AH::Common::MonsterData::MagicalImmunity)) {
        dmgMag = 0;
    }
    if (m->attributes().testFlag(AH::Common::MonsterData::MagicalResistance)) {
        // rounded up half
        dmgMag = (dmgMag+1) / 2;
    }

    // Calculate final pool
    int dmgTotal = dmgBase + dmgGen + dmgMag + dmgPhy;
    ModifiedPropertyValue pool(PropertyValue(PropertyValue::Skill_Combat, dmgBase), dmgTotal, base.modifiers()+gen+mag+phy);

    ModifiedPropertyValue clueBurnMods = gGame->context().getCurCharacterClueBurn(AH::Skill_Combat);

    DieTestHelper::DieTestSpec test = DieTestHelper::createClueBurnTest("Attack Monster", p->getCharacter(), pool, clueBurnMods, m->combatAdjustment(), m->toughness());
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(p, test);
    if (res.boolResult) {
        m_fight->updatePhaseByResult(FightPhase::MonsterKilled);
        m->defeat(p->getCharacter());
    } else {
        m_fight->updatePhaseByResult(FightPhase::AttackFailed);
    }
    return true;
}

void AttackAction::filterEquipped(PropertyModificationList &lst)
{
    for (PropertyModificationList::iterator it = lst.begin(); it != lst.end();) {
        if (const GameObject * obj = dynamic_cast<const GameObject *> (it->getModifier())) {
            if (!obj->isEquipped()) {
                it = lst.erase(it);
                continue;
            }
        }

        ++it;
    }
}
