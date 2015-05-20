#include "playerdata.h"

namespace AH { namespace Common {

PlayerData::PlayerData()
{
}


void operator <<(QVariant &data, const PlayerData &c) {
    QVariantMap map;
    map["id"] << c.m_id;
    map["investigatorId"] << c.m_invId;
    data << map;
}

void operator >>(const QVariant &data, PlayerData &c) {
    QVariantMap map;
    data >> map;
    map["id"] >> c.m_id;
    map["investigatorId"] >> c.m_invId;
}

}}
