#include "gamefielddata.h"

namespace AH { namespace Common {

GameFieldData::GameFieldData()
:   m_clues(0),
    m_sealed(false)
{
}

void operator <<(QVariant &data, const GameFieldData &target)
{
    QVariantMap map;
    map["id"] << target.id();
    map["name"] << target.name();
    map["type"] << target.type();
    map["neighbours"] << target.neighbourIds();
    map["clueAmount"] << target.clueAmount();
    map["isSealed"] << target.isSealed();
    map["characters"] << target.characterIds();
    map["monsters"] << target.monsterIds();
    map["gate"] << target.gateId();
    data << map;
}

void operator >>(const QVariant &data, GameFieldData &target)
{
    QVariantMap map;
    data >> map;
    map["id"] >> target.m_id;
    map["name"] >> target.m_name;
    map["type"] >> target.m_type;
    map["neighbours"] >> target.m_neighbourIds;
    map["clueAmount"] >> target.m_clues;
    map["isSealed"] >> target.m_sealed;
    map["characters"] >> target.m_characterIds;
    map["monsters"] >> target.m_monsterIds;
    map["gate"] >> target.m_gateId;
}

}}
