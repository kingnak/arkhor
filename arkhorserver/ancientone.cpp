#include "ancientone.h"
#include "game/game.h"

AncientOne::AncientOne()
    : m_awake(false), m_dirty(false), m_damageThreshold(1), m_damage(0)
{
}

void AncientOne::increaseDoomTrack(int amount)
{
    m_doomValue += amount;
    setDirty();
}

PropertyModificationList AncientOne::getCombatModifications() const
{
    PropertyModificationList mods;
    AH::Common::MonsterData::MonsterAttributes attrs = defenses();
    //attrs &= ~ignoredAttributes;

    if (attrs.testFlag(AH::Common::MonsterData::PhysicalImmunity)) {
        mods << PropertyModification(this, PropertyValue::Damage_Physical, 0, PropertyModification::Setting);
    }
    if (attrs.testFlag(AH::Common::MonsterData::PhysicalResistance)) {
        // rounded up half
        mods << PropertyModification(this, PropertyValue::Damage_Physical, 2, PropertyModification::DividingUp);
    }
    if (attrs.testFlag(AH::Common::MonsterData::MagicalImmunity)) {
        mods << PropertyModification(this, PropertyValue::Damage_Magical, 0, PropertyModification::Setting);
    }
    if (attrs.testFlag(AH::Common::MonsterData::MagicalResistance)) {
        // rounded up half
        mods << PropertyModification(this, PropertyValue::Damage_Magical, 2, PropertyModification::DividingUp);
    }
    return mods;
}

PropertyModificationList AncientOne::getModifications() const
{
    if (m_awake) {
        return getCombatModifications();
    } else {
        return getSlumberModifications();
    }
}

void AncientOne::awake()
{
    m_doomValue = m_doomTrack;
    m_damageThreshold = gGame->getPlayers().count();
    m_damage = 0;
    m_awake = true;
}

void AncientOne::newAttackRound()
{
    decreaseAttackAdjustment(1);
}

bool AncientOne::isDefeated() const
{
    if (m_awake && m_doomValue <= 0) {
        return true;
    }
    return false;
}

void AncientOne::damage(int amount)
{
    m_damage += amount;
    int dec = m_damage / m_damageThreshold;
    m_damage %= m_damageThreshold;
    decreaseDoomTrack(dec);
}
