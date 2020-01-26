#include "playerdata.h"

namespace AH { namespace Common {

DEFINE_ENUM_SERIALIZER(PlayerData::AutoSkipData)

PlayerData::PlayerData()
    : m_autoSkip(NoAutoChoose)
{
}


void operator <<(QVariant &data, const PlayerData &c) {
    QVariantMap map;
    map["id"] << c.m_id;
    map["investigatorId"] << c.m_invId;
    map["autoSkip"] << c.m_autoSkip;
    data << map;
}

void operator >>(const QVariant &data, PlayerData &c) {
    QVariantMap map;
    data >> map;
    map["id"] >> c.m_id;
    map["investigatorId"] >> c.m_invId;
    map["autoSkip"] >> c.m_autoSkip;
}

}}
