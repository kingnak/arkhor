#include "monster.h"
#include "game/gamefield.h"
#include "character.h"

AH::Common::MonsterData *Monster::data()
{
    // Synchronize data
    m_fieldId = (m_field) ? m_field->id() : AH::Common::FieldData::NO_NO_FIELD;
    m_attributes = attributes();
    return AH::Common::MonsterData::data();
}

AH::Common::MonsterData::DisplayType Monster::baseDisplayType() const
{
    if (movementType() == Special)
        return MinimalBack;
    return OnlyFront;
}

int Monster::awareness() const
{
    return gGame->context().getMonsterProperty(this, PropertyValue::Monster_Awareness).finalVal();
}

int Monster::horrorAdjustment() const
{
    return gGame->context().getMonsterProperty(this, PropertyValue::Monster_HorrorAdjustment).finalVal();
}

int Monster::horrorDamage() const
{
    return gGame->context().getMonsterProperty(this, PropertyValue::Monster_HorrorDamage).finalVal();
}

int Monster::combatAdjustment() const
{
    return gGame->context().getMonsterProperty(this, PropertyValue::Monster_CombatAdjustment).finalVal();
}

int Monster::combatDamage() const
{
    return gGame->context().getMonsterProperty(this, PropertyValue::Monster_CombatDamage).finalVal();
}

int Monster::toughness() const
{
    return gGame->context().getMonsterProperty(this, PropertyValue::Monster_Toughness).finalVal();
}

AH::Common::MonsterData::DisplayType Monster::displayType() const
{
    //return gGame->context().getMonsterDisplayType(this);
    return baseDisplayType();
}

AH::Common::MonsterData::MovementType Monster::movementType() const
{
    return static_cast<AH::Common::MonsterData::MovementType> (gGame->context().getMonsterProperty(this, PropertyValue::Monster_Movement).finalVal());
}

void Monster::move(AH::MovementDirection dir)
{
    // Don't move if not on board
    if (!m_field) {
        return;
    }

    // Never move away from characters
    if (m_field->hasCharacters()) {
        return;
    }

    GameField *curField = m_field;
    MovementType movementType = this->movementType();
    switch (movementType) {
    case Stationary:
        return;
    case Fast:
        // Move 2 fields
        if (dir == AH::MoveBlack) {
            curField = curField->blackField();
        } else {
            curField = curField->whiteField();
        }
        // Stop after 1 field if there is a character
        if (curField->hasCharacters()) {
            curField->placeMonster(this);
            break;
        }
        // Fallthrough to move to next field
    case Normal:
        if (dir == AH::MoveBlack) {
            curField = curField->blackField();
        } else {
            curField = curField->whiteField();
        }
        curField->placeMonster(this);
        break;

    case Flying:
        // Case where monster is on a field with character already handled above.
        // Case 1: Sky. Search all streets
        // Case 2: Not sky. Search all connected streets
        // Find character with minimum Sneak
        // On tie, randomize
    {
        QList<GameField*> checkFields;
        if (curField->type() == AH::Common::FieldData::Sky) {
            checkFields = gGame->board()->fields(AH::Common::FieldData::Street);
        } else {
            for (auto f : curField->neighbours().toList()) {
                if (f->type() == AH::Common::FieldData::Street) {
                    checkFields << f;
                }
            }
        }

        // Find field with characters of min sneak
        int minSneak = std::numeric_limits<int>::max();
        QSet<GameField*> candidates;
        for (auto f : checkFields) {
            for (auto c : f->characters()) {
                int curSneak = gGame->context().getCharacterSkill(c, AH::Skill_Sneak).finalVal();
                if (curSneak == minSneak) {
                    candidates.insert(f);
                } else if (curSneak < minSneak) {
                    candidates.clear();
                    minSneak = curSneak;
                    candidates.insert(f);
                }
            }
        }

        if (candidates.count() > 0) {
            int i = RandomSource::instance().nextUint(0, candidates.count()-1);
            (*(candidates.begin()+i))->placeMonster(this);
        } else if (curField->type() != AH::Common::FieldData::Sky) {
            gGame->board()->field(AH::Common::FieldData::Sp_Sky)->placeMonster(this);
        }
    }
        break;
    case Special:
        Q_ASSERT_X(false, "Monster::move", "Cannot handle special movement here");
    }
}

void Monster::defeat(Character *byCharacter)
{
    if (m_field) {
        m_field->removeMonster(this);
    }

    if (attributes().testFlag(Endless)) {
        if (!gGame->context().checkCharacterIgnoreMonsterAttribute(byCharacter, Endless)) {
            // Back to monter pool
            gGame->returnMonster(this);
            return;
        }
    }
    byCharacter->addMonsterTrophy(this);
}

void Monster::endCombat()
{
    if (isSpontaneous()) {
        if (m_field) {
            m_field->removeMonster(this);
        }
        // Back to monster pool
        gGame->returnMonster(this);
    }
}

PropertyModificationList Monster::getModifications() const
{
    return getFilteredModifications(0);
}

PropertyModificationList Monster::getFilteredModifications(AH::Common::MonsterData::MonsterAttributes ignoredAttributes) const
{
    PropertyModificationList mods;
    MonsterAttributes attrs = attributes();
    attrs &= ~ignoredAttributes;

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
