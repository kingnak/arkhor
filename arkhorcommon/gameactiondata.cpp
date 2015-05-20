#include "gameactiondata.h"


namespace AH { namespace Common {

GameActionData::GameActionData()
{
}

void operator <<(QVariant &data, const GameActionData &target)
{
    QVariantMap map;
    map["id"] << target.id();
    map["name"] << target.name();
    map["description"] << target.description();
    map["sourceId"] << target.sourceId();
    map["phases"] << target.phases();
    data << map;
}

void operator >>(const QVariant &data, GameActionData &target)
{
    QVariantMap map;
    data >> map;
    map["id"] >> target.m_id;
    map["name"] >> target.m_name;
    map["description"] >> target.m_description;
    map["sourceId"] >> target.m_sourceId;
    map["phases"] >> target.m_phases;
}

}}
