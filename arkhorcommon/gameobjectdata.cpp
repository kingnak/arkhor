#include "gameobjectdata.h"

namespace AH { namespace Common {

GameObjectData::GameObjectData()
    : m_bExhaustable(false), m_bIsExhausted(false)
{
}

void operator <<(QVariant &data, const GameObjectData &target)
{
    QVariantMap map;
    map["id"] << target.id();
    map["name"] << target.name();
    map["description"] << target.description();
    map["type"] << target.type();
    map["hands"] << target.handCount();
    map["price"] << target.price();
    map["exhaustable"] << target.m_bExhaustable;
    map["isExhausted"] << target.isExhausted();
    map["actionIds"] << target.actionIds();
    map["optionIds"] << target.optionIds();
    data << map;
}

void operator >>(const QVariant &data, GameObjectData &target)
{
    QVariantMap map;
    data >> map;
    map["id"] >> target.m_id;
    map["name"] >> target.m_name;
    map["description"] >> target.m_description;
    map["type"] >> target.m_type;
    map["hands"] >> target.m_hands;
    map["price"] >> target.m_price;
    map["exhaustable"] >> target.m_bExhaustable;
    map["isExhausted"] >> target.m_bIsExhausted;
    map["actionIds"] >> target.m_actionIds;
    map["optionIds"] >> target.m_optionIds;
}

}}
