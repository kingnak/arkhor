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
    ~Monster() override {}

    virtual Monster *clone() = 0;

    virtual void setId(QString id) { m_id = id; }

    MonsterData *data() override;
    int baseAwareness() const { return m_awareness; }
    int baseHorrorAdjustment() const { return m_horrorAdjustment; }
    int baseHorrorDamage() const { return m_horrorDamage; }
    int baseCombatAdjustment() const { return m_combatAdjustment; }
    int baseCombatDamage() const { return m_combatDamage; }
    int baseToughness() const { return m_toughness; }
    DisplayType baseDisplayType() const;
    MovementType baseMovement() const { return m_movement; }

    int awareness() const override;
    int horrorAdjustment() const override;
    int horrorDamage() const override;
    int combatAdjustment() const override;
    int combatDamage() const override;
    int toughness() const override;
    DisplayType displayType() const override;
    MovementType movementType() const override;

    void setDimension(AH::Dimension dim) { m_dimension = dim; }

    GameField *field() { return m_field; }
    void setField(GameField *f) { m_field = f; }
    AH::Common::FieldData::FieldID fieldId() const override;

    bool isSpontaneous() const { return m_isSpontaneous; }
    void setSpontaneous(bool spontaneous = true) { m_isSpontaneous = spontaneous; }

    void returnToDeck(bool notifyChange = false);
    virtual bool hasCustomHorrorCheck() const { return false; }
    virtual bool horrorCheck(Character *c) { Q_UNUSED(c) return true; }
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

    QString modifierId() const override { return id(); }
    PropertyModificationList getModifications() const override;
    PropertyModificationList getFilteredModifications(MonsterAttributes ignoredAttributes) const;

protected:
    void execDefeat(Character *byCharacter);

protected:
    GameField *m_field;
    bool m_isSpontaneous;
};

#endif // MONSTER_H
