#ifndef MONSTERMODIFIER_H
#define MONSTERMODIFIER_H

#include "monster.h"
#include "propertymodifier.h"

class MonsterModifierScript;

class MonsterModifier
{
public:
    MonsterModifier() {}
    virtual ~MonsterModifier() {}

    virtual PropertyModificationList getMonsterModifications(const AH::Common::MonsterData &monster, bool matchAllAttributes = false) const;
    virtual PropertyModificationList getMonsterModifications(QString typeId) const;
    virtual PropertyModificationList getMonsterModifications(AH::Common::MonsterData::MonsterAttributes attrs, bool matchAll = false) const;

    virtual PropertyModificationList getMonsterMovementModifications(const Monster &monster) const;
    virtual AH::Common::MonsterData::MovementType getMonsterMovementModifications(QString typeId, AH::Common::MonsterData::MovementType origMove) const;
    virtual AH::Common::MonsterData::MovementType getMonsterMovementModifications(AH::Common::MonsterData::MovementType move) const;

protected:
    void addModifications(QString type, PropertyModificationList mods);
    void addModifications(AH::Common::MonsterData::MonsterAttributes attrs, PropertyModificationList mods);

    void addMoveModification(QString type, AH::Common::MonsterData::MovementType mov);
    void addMoveModification(AH::Common::MonsterData::MovementType movOld, AH::Common::MonsterData::MovementType movNew);

    virtual const PropertyModifier *getMonsterMovementModifier() const = 0;

private:
    QMap<QString, PropertyModificationList> m_typeMods;
    QMap<AH::Common::MonsterData::MonsterAttributes, PropertyModificationList> m_attrMods;

    QMap<QString, AH::Common::MonsterData::MovementType> m_typeMov;
    QMap<AH::Common::MonsterData::MovementType, AH::Common::MonsterData::MovementType> m_moveMov;

    friend class MonsterModifierScript;
};

#endif // MONSTERMODIFIER_H
