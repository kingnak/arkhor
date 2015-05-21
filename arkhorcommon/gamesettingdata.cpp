#include "gamesettingdata.h"

namespace AH { namespace Common {

GameSettingData::GameSettingData(int tl, QString aoId, QString rId, QString eId)
:   m_terror(tl),
    m_ancientOneId(aoId),
    m_rumorId(rId),
    m_environmentId(eId)
{
}

void operator <<(QVariant &data, const GameSettingData &target)
{
    QVariantMap map;
    map["terror"] << target.terrorLevel();
    map["ancientOneId"] << target.ancientOneId();
    map["rumorId"] << target.rumorId();
    map["environmentId"] << target.environmentId();
    data << map;
}

void operator >>(const QVariant &data, GameSettingData &target)
{
    QVariantMap map;
    data >> map;
    map["terror"] >> target.m_terror;
    map["ancientOneId"] >> target.m_ancientOneId;
    map["rumorId"] >> target.m_rumorId;
    map["environmentId"] >> target.m_environmentId;
}

}}
