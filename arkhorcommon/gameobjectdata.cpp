#include "gameobjectdata.h"

namespace AH { namespace Common {

GameObjectData::GameObjectData()
:   m_type(AH::NoObject),
    m_hands(0),
    m_price(0),
    m_bExhaustable(false),
    m_bIsExhausted(false),
    m_isEquipped(false),
    m_castCost(0),
    m_castAdjustment(0),
    m_castTarget(0)
{
}

void operator <<(QVariant &data, const GameObjectData &target)
{
    QVariantMap map;
    map["id"] << target.id();
    map["tid"] << target.typeId();
    map["name"] << target.name();
    map["description"] << target.description();
    map["type"] << target.type();
    map["hands"] << target.handCount();
    map["price"] << target.price();
    map["exhaustable"] << target.m_bExhaustable;
    map["isExhausted"] << target.isExhausted();
    map["actionIds"] << target.actionIds();
    map["optionIds"] << target.optionIds();
    map["equipped"] << target.isEquipped();
    map["castCost"] << target.castCost();
    map["castAdjustment"] << target.castAdjustment();
    map["castTarget"] << target.castTarget();
    data << map;
}

void operator >>(const QVariant &data, GameObjectData &target)
{
    QVariantMap map;
    data >> map;
    map["id"] >> target.m_id;
    map["tid"] >> target.m_tid;
    map["name"] >> target.m_name;
    map["description"] >> target.m_description;
    map["type"] >> target.m_type;
    map["hands"] >> target.m_hands;
    map["price"] >> target.m_price;
    map["exhaustable"] >> target.m_bExhaustable;
    map["isExhausted"] >> target.m_bIsExhausted;
    map["actionIds"] >> target.m_actionIds;
    map["optionIds"] >> target.m_optionIds;
    map["equipped"] >> target.m_isEquipped;
    map["castCost"] >> target.m_castCost;
    map["castAdjustment"] >> target.m_castAdjustment;
    map["castTarget"] >> target.m_castTarget;
}

}}
