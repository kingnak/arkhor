#include "diedata.h"

namespace AH { namespace Common {

DEFINE_ENUM_SERIALIZER(DiePoolData::PoolType)
DEFINE_ENUM_SERIALIZER(DieRollData::RollType)
DEFINE_ENUM_SERIALIZER(DieRollTestData::TestType)

void operator <<(QVariant &data, const DiePoolData &target)
{
    QVariantMap map;
    map["type"] << target.m_type;
    map["dieCount"] << target.m_count;
    map["adjustment"] << target.m_adjust;
    map["baseProperty"] << target.m_property;
    map["dieValues"] << target.m_dieVals;
    data << map;
}

void operator >>(const QVariant &data, DiePoolData &target)
{
    QVariantMap map;
    data >> map;
    map["type"] >> target.m_type;
    map["dieCount"] >> target.m_count;
    map["adjustment"] >> target.m_adjust;
    map["baseProperty"] >> target.m_property;
    map["dieValues"] >> target.m_dieVals;
}

void operator <<(QVariant &data, const DieRollData &target)
{
    QVariantMap map;
    map["type"] << target.m_type;
    map["diePool"] << target.m_pool;
    map["successValues"] << target.m_successes;
    data << map;
}

void operator >>(const QVariant &data, DieRollData &target)
{
    QVariantMap map;
    data >> map;
    map["type"] >> target.m_type;
    map["diePool"] >> target.m_pool;
    map["successValues"] >> target.m_successes;
}

void operator <<(QVariant &data, const DieRollTestData &target)
{
    QVariantMap map;
    map["type"] << target.m_type;
    map["dieRoller"] << target.m_roller;
    map["targetValue"] << target.m_target;
    map["diceForClue"] << target.m_diceForClue;
    map["options"] << target.m_options;
    map["clueBurnMods"] << target.m_clueBurnMods;
    map["description"] << target.m_desc;
    map["sourceId"] << target.m_sourceId;
    map["succeeded"] << target.m_succeeded;
    map["generalMods"] << target.m_generalMods;
    data << map;
}

void operator >>(const QVariant &data, DieRollTestData &target)
{
    QVariantMap map;
    data >> map;
    map["type"] >> target.m_type;
    map["dieRoller"] >> target.m_roller;
    map["targetValue"] >> target.m_target;
    map["diceForClue"] >> target.m_diceForClue;
    map["options"] >> target.m_options;
    map["clueBurnMods"] >> target.m_clueBurnMods;
    map["description"] >> target.m_desc;
    map["sourceId"] >> target.m_sourceId;
    map["succeeded"] >> target.m_succeeded;
    map["generalMods"] >> target.m_generalMods;
}

void operator <<(QVariant &data, const DieTestUpdateData &target)
{
    QVariantMap map;
    map["optionId"] << target.m_optionId;
    map["clueBurns"] << target.m_clueBurnAmount;
    data << map;
}

void operator >>(const QVariant &data, DieTestUpdateData &target)
{
    QVariantMap map;
    data >> map;
    map["optionId"] >> target.m_optionId;
    map["clueBurns"] >> target.m_clueBurnAmount;
}

}}
