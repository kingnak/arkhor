#ifndef MONSTER_H
#define MONSTER_H

#include <QString>
#include <monsterdata.h>
#include "game/propertymodifier.h"

class Character;
class GameField;

class Monster : public AH::Common::MonsterData, public PropertyModifier
{
public:
    Monster() : m_field(NULL), m_isSpontaneous(false) {}
    virtual ~Monster() {}

    virtual Monster *clone() = 0;

    virtual void setId(QString id) { m_id = id; }

    virtual MonsterData *data();
    int baseAwareness() const { return m_awareness; }
    int baseHorrorAdjustment() const { return m_horrorAdjustment; }
    int baseHorrorDamage() const { return m_horrorDamage; }
    int baseCombatAdjustment() const { return m_combatAdjustment; }
    int baseCombatDamage() const { return m_combatDamage; }
    int baseToughness() const { return m_toughness; }
    MovementType baseMovement() const { return m_movement; }

    virtual int awareness() const;
    virtual int horrorAdjustment() const;
    virtual int horrorDamage() const;
    virtual int combatAdjustment() const;
    virtual int combatDamage() const;
    virtual int toughness() const;
    virtual MovementType movementType() const;

    void setDimension(AH::Dimension dim) { m_dimension = dim; }

    GameField *field() { return m_field; }
    void setField(GameField *f) { m_field = f; }

    bool isSpontaneous() const { return m_isSpontaneous; }
    void setSpontaneous(bool spontaneous = true) { m_isSpontaneous = spontaneous; }

    virtual void move(AH::MovementDirection dir) = 0;
    virtual void defeat(Character *byCharacter);
    virtual void endCombat();

    enum DamageType {
        Horror,
        Combat,
        Nightmare,
        Overwhelm
    };

    virtual bool damage(Character *c, DamageType t) {Q_UNUSED(c) Q_UNUSED(t) return true;}
    virtual void evaded(Character *c) {Q_UNUSED(c)}
    virtual void flown(Character *c) {Q_UNUSED(c)}

    virtual QString modifierId() const { return id(); }
    virtual PropertyModificationList getModifications() const;
    PropertyModificationList getFilteredModifications(MonsterAttributes ignoredAttributes) const;

protected:
    GameField *m_field;
    bool m_isSpontaneous;
};

#endif // MONSTER_H
