#include "monsterdata.h"

namespace AH { namespace Common {

DEFINE_ENUM_SERIALIZER(MonsterData::MovementType)
DEFINE_ENUM_SERIALIZER(MonsterData::MonsterAttribute)
DEFINE_ENUM_SERIALIZER(MonsterData::MonsterAttributes)

MonsterData::MonsterData()
:   m_dimension(AH::NoDimension),
    m_movement(Normal),
    m_awareness(0),
    m_horrorAdjustment(0),
    m_horrorDamage(0),
    m_combatAdjustment(0),
    m_combatDamage(0),
    m_toughness(0),
    m_fieldId(FieldData::NO_NO_FIELD)
{
}

void operator << (QVariant &data, const MonsterData &target)
{
    QVariantMap map;
    map["id"] << target.id();
    map["tid"] << target.typeId();
    map["name"] << target.name();
    map["desc"] << target.description();
    map["mythText"] << target.mythText();
    map["dimension"] << target.dimension();
    map["movement"] << target.movementType();
    map["awareness"] << target.awareness();
    map["horrorAdjustment"] << target.horrorAdjustment();
    map["horrorDamage"] << target.horrorDamage();
    map["combatAdjustment"] << target.combatAdjustment();
    map["combatValue"] << target.combatDamage();
    map["toughness"] << target.toughness();
    map["attributes"] << target.attributes();
    data << map;
}

void operator >> (const QVariant &data, MonsterData &target)
{
    QVariantMap map;
    data >> map;
    map["id"] >> target.m_id;
    map["tid"] >> target.m_tid;
    map["name"] >> target.m_name;
    map["desc"] >> target.m_desc;
    map["mythText"] >> target.m_mythText;
    map["dimension"] >> target.m_dimension;
    map["movement"] >> target.m_movement;
    map["awareness"] >> target.m_awareness;
    map["horrorAdjustment"] >> target.m_horrorAdjustment;
    map["horrorDamage"] >> target.m_horrorDamage;
    map["combatAdjustment"] >> target.m_combatAdjustment;
    map["combatValue"] >> target.m_combatDamage;
    map["toughness"] >> target.m_toughness;
    map["attributes"] >> target.m_attributes;
}

}}
