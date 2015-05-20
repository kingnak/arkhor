#include "ancientonedata.h"

namespace AH { namespace Common {

AncientOneData::AncientOneData()
    : m_defenses(0), m_doomTrack(0), m_combatAdjustment(0)
{
}

void operator <<(QVariant &data, const AncientOneData &target)
{
    QVariantMap map;
    map["id"] << target.id();
    map["name"] << target.name();
    map["worshippersText"] << target.worshippersText();
    map["powerText"] << target.powerText();
    map["battleStartText"] << target.battleStartText();
    map["attackText"] << target.attackText();
    map["defenses"] << target.defenses();
    map["doomTrack"] << target.doomTrack();
    map["combatAdjustment"] << target.combatAdjustment();
    data << map;
}

void operator >>(const QVariant &data, AncientOneData &target)
{
    QVariantMap map;
    data >> map;
    map["id"] >> target.m_id;
    map["name"] >> target.m_name;
    map["worshippersText"] >> target.m_worshippersText;
    map["powerText"] >> target.m_powerText;
    map["battleStartText"] >> target.m_battleStartText;
    map["attackText"] >> target.m_attackText;
    map["defenses"] >> target.m_defenses;
    map["doomTrack"] >> target.m_doomTrack;
    map["combatAdjustment"] >> target.m_combatAdjustment;
}

}}
