#include "monstermodifier.h"

using namespace AH::Common;

PropertyModificationList MonsterModifier::getMonsterModifications(const MonsterData &monster, bool matchAllAttributes) const
{
    PropertyModificationList ret = getMonsterModifications(monster.typeId());
    ret << getMonsterModifications(monster.attributes(), matchAllAttributes);
    return ret;
}

PropertyModificationList MonsterModifier::getMonsterModifications(const QString &typeId) const
{
    PropertyModificationList ret = m_typeMods[typeId];
    if (typeId != "*")
        ret << m_typeMods["*"];
    return ret;
}

PropertyModificationList MonsterModifier::getMonsterModifications(MonsterData::MonsterAttributes attrs, bool matchAll) const
{
    PropertyModificationList ret;
    for (auto k : m_attrMods.keys()) {
        if (matchAll) {
            if ((attrs & k) == attrs) {
                ret.append(m_attrMods[k]);
            }
        } else {
            if ((attrs & k) != 0) {
                ret.append(m_attrMods[k]);
            }
        }
    }

    return ret;
}

PropertyModificationList MonsterModifier::getMonsterMovementModifications(const Monster &monster) const
{
    // Map type specific, then movement -> movement
    bool mapped = false;
    MonsterData::MovementType m = monster.baseMovement();
    if (m_typeMov.contains(monster.typeId())) {
        mapped = true;
        m = m_typeMov[monster.typeId()];
    }
    if (m_moveMov.contains(m)) {
        mapped = true;
        m = m_moveMov[m];
    }

    PropertyModificationList ret;
    if (mapped) {
        ret << PropertyModification(this->getMonsterMovementModifier(), PropertyValue::Monster_Movement, m, PropertyModification::Setting);
    }
    return ret;
}

MonsterData::MovementType MonsterModifier::getMonsterMovementModifications(const QString &typeId, MonsterData::MovementType origMove) const
{
    if (m_typeMov.contains(typeId))
        return m_typeMov[typeId];
    return origMove;
}

MonsterData::MovementType MonsterModifier::getMonsterMovementModifications(MonsterData::MovementType move) const
{
    if (m_moveMov.contains(move))
        return m_moveMov[move];
    return move;
}

bool MonsterModifier::hasMonsterModifications() const
{
    return !m_typeMods.isEmpty() ||
            !m_attrMods.isEmpty() ||
            !m_typeMov.isEmpty() ||
            !m_moveMov.isEmpty();
}

void MonsterModifier::addModifications(const QString &type, const PropertyModificationList &mods)
{
    m_typeMods[type] << mods;
}

void MonsterModifier::addModifications(MonsterData::MonsterAttributes attrs, const PropertyModificationList &mods)
{
    m_attrMods[attrs] << mods;
}

void MonsterModifier::addMoveModification(const QString &type, MonsterData::MovementType mov)
{
    m_typeMov[type] = mov;
}

void MonsterModifier::addMoveModification(MonsterData::MovementType movOld, MonsterData::MovementType movNew)
{
    m_moveMov[movOld] = movNew;
}
