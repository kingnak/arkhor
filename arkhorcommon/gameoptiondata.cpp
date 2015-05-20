#include "gameoptiondata.h"

namespace AH { namespace Common {

GameOptionData::GameOptionData()
    : m_isAvailable(true), m_canPay(true)
{
}

GameOptionData::~GameOptionData()
{
}

void operator <<(QVariant &data, const GameOptionData &target)
{
    QVariantMap map;
    map["id"] << target.id();
    map["name"] << target.name();
    map["description"] << target.description();
    map["sourceId"] << target.sourceId();
    map["phases"] << target.phases();
    map["actionId"] << target.actionId();
    map["costs"] << target.costs();
    map["chooseType"] << target.chooseType();
    map["continueType"] << target.continueType();
    map["available"] << target.isAvailable();
    map["canPay"] << target.canPay();
    data << map;
}

void operator >>(const QVariant &data, GameOptionData &target)
{
    QVariantMap map;
    data >> map;
    map["id"] >> target.m_id;
    map["name"] >> target.m_name;
    map["description"] >> target.m_description;
    map["sourceId"] >> target.m_sourceId;
    map["phases"] >> target.m_phases;
    map["actionId"] >> target.m_actionId;
    map["costs"] >> target.m_costs;
    map["chooseType"] >> target.m_chooseType;
    map["continueType"] >> target.m_continueType;
    map["available"] >> target.m_isAvailable;
    map["canPay"] >> target.m_canPay;
}

}}
