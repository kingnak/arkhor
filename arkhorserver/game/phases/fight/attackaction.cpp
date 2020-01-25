#include "attackaction.h"
#include "game/gameobject.h"
#include "game/game.h"
#include "game/player.h"
#include "game/dietesthelper.h"
#include "character.h"
#include "monster.h"
#include "fightphase.h"

using namespace AH::Common;

AttackAction::AttackAction(FightPhase *fight)
    : m_fight(fight)
{
}

bool AttackAction::execute()
{
    Monster *m = gGame->context().monster();
    Player *p = gGame->context().player();

    gGame->notifier()->actionStart(this);

    auto base = getAttackModifications(p->getCharacter());
    ModifiedPropertyValue pool = base.finalPropery();

    ModifiedPropertyValue clueBurnMods = gGame->context().getCurCharacterClueBurn(AH::Skill_Combat);

    DieTestHelper::DieTestSpec test = DieTestHelper::createClueBurnTest("Attack Monster", m->id(), p->getCharacter(), pool, clueBurnMods, m->combatAdjustment(), m->toughness());
    DieTestHelper::addDieRollOptions(test, p->getCharacter(), AH::Skill_Combat);
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(p, test);

    // TODO: Call post-use methods for equipped objects ==> Needed?

    // Remove single use objects
    discardAfterAttack(base.weaponsGeneral);
    discardAfterAttack(base.weaponsPhysical);
    discardAfterAttack(base.weaponsMagical);

    if (res.boolResult) {
        gGame->notifier()->actionUpdate(this, "succeeded");
        m_fight->updatePhaseByResult(FightPhase::MonsterKilled);
        m->defeat(p->getCharacter());
    } else {
        gGame->notifier()->actionUpdate(this, "failed");
        m_fight->updatePhaseByResult(FightPhase::AttackFailed);
    }

    gGame->notifier()->actionFinish(this);

    return true;
}

QString AttackAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(desc);
    switch (part) {
    case Start:
        return "{C} attacks {M}";
    case Finish:
        return "Attack {B}";
    default:
        return QString::null;
    }
}

void AttackAction::filterEquipped(PropertyModificationList &lst) const
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

void AttackAction::discardAfterAttack(PropertyModificationList &lst)
{
    // Collect which objects to discard
    QList<const GameObject *> toDiscard;
    foreach (PropertyModification p, lst) {
        if (const GameObject *obj = dynamic_cast<const GameObject *> (p.getModifier())) {
            if (obj->getAttributes().testFlag(GameObject::DiscardAfterAttack)) {
                toDiscard << obj;
            }
        }
    }

    // Go through inventory and remove
    foreach (const GameObject *obj, toDiscard) {
        // Work on Inventory Copy
        QList<GameObject *> inv = gGame->context().player()->getCharacter()->inventory();
        inv.detach();
        foreach (GameObject *iObj, inv) {
            if (obj == iObj) {
                // Remove it
                gGame->returnObject(iObj);
            }
        }
    }
}

PropertyModificationList AttackAction::getMonsterModifications(Character *c) const
{
    MonsterData::MonsterAttributes ignoredAttributes = gGame->context().getCharacterIgnoredMonsterAttributes(c);
    return gGame->context().monster()->getFilteredModifications(ignoredAttributes);
}

AttackAction::AttackModifications AttackAction::getAttackModifications(Character *c, bool asDieRoll) const
{
    // Get base damage + weapon damages
    ModifiedPropertyValue base = asDieRoll
            ? gGame->context().getCharacterSkill(c, AH::Skill_Combat)
            : gGame->context().getCharacterDieRoll(c, PropertyValue::Property::DieRoll_Combat);
    PropertyModificationList gen = c->getPropertyModifiers().filtered(PropertyValue::Damage_General);
    PropertyModificationList mag = c->getPropertyModifiers().filtered(PropertyValue::Damage_Magical);
    PropertyModificationList phy = c->getPropertyModifiers().filtered(PropertyValue::Damage_Physical);

    // Get only equipped
    filterEquipped(gen);
    filterEquipped(mag);
    filterEquipped(phy);

    PropertyModificationList monsterMods = this->getMonsterModifications(c);

    return AttackModifications({base, gen, phy, mag, monsterMods.filtered(PropertyValue::Damage_Physical), monsterMods.filtered(PropertyValue::Damage_Magical)});
}

ModifiedPropertyValue AttackAction::AttackModifications::finalPropery() const
{
    auto phy = weaponsPhysical+monsterPhysical;
    auto mag = weaponsMagical+monsterMagical;
    int dmgBase = base.finalVal();
    int dmgGen = weaponsGeneral.apply(0);
    int dmgPhy = phy.apply(0);
    int dmgMag = mag.apply(0);
    auto ret = ModifiedPropertyValue(PropertyValue(base.property(), base.base()), weaponsGeneral+phy+mag);
    ret.overrideFinalValue(dmgBase+dmgGen+dmgPhy+dmgMag);
    return ret;
}

ModifiedPropertyValue AttackAction::AttackModifications::characterProperty() const
{
    auto phy = weaponsPhysical;
    auto mag = weaponsMagical;
    int dmgBase = base.finalVal();
    int dmgGen = weaponsGeneral.apply(0);
    int dmgPhy = phy.apply(0);
    int dmgMag = mag.apply(0);
    auto ret = ModifiedPropertyValue(PropertyValue(base.property(), base.base()), weaponsGeneral+phy+mag);
    ret.overrideFinalValue(dmgBase+dmgGen+dmgPhy+dmgMag);
    return ret;
}

///////////////////////////////////////

QString AttackOption::sourceId() const
{
    return gGame->context().monster()->id();
}

ModifiedPropertyValueData AttackOption::baseProperty() const
{
    auto prop = aa.getAttackModifications(gGame->context().player()->getCharacter(), true);
    ModifiedPropertyValue base = prop.characterProperty();
    return base.toModifiedPropertyValueData();
}

