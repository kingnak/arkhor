#include "ancientonedata.h"

namespace AH { namespace Common {

AncientOneData::AncientOneData()
    : m_defenses(0)
    , m_doomTrack(0)
    , m_doomValue(0)
    , m_combatAdjustment(0)
    , m_attackAdjustment(0)
{
}

void operator <<(QVariant &data, const AncientOneData &target)
{
    QVariantMap map;
    map["id"] << target.id();
    map["name"] << target.name();
    map["worshippersText"] << target.worshippersText();
    map["powerTitle"] << target.powerTitle();
    map["powerText"] << target.powerText();
    map["battleStartText"] << target.battleStartText();
    map["attackText"] << target.attackText();
    map["defenses"] << target.defenses();
    map["doomTrack"] << target.doomTrack();
    map["doomValue"] << target.doomValue();
    map["combatAdjustment"] << target.combatAdjustment();
    map["attackAdjustment"] << target.attackAdjustment();
    data << map;
}

void operator >>(const QVariant &data, AncientOneData &target)
{
    QVariantMap map;
    data >> map;
    map["id"] >> target.m_id;
    map["name"] >> target.m_name;
    map["worshippersText"] >> target.m_worshippersText;
    map["powerTitle"] >> target.m_powerTitle;
    map["powerText"] >> target.m_powerText;
    map["battleStartText"] >> target.m_battleStartText;
    map["attackText"] >> target.m_attackText;
    map["defenses"] >> target.m_defenses;
    map["doomTrack"] >> target.m_doomTrack;
    map["doomValue"] >> target.m_doomValue;
    map["combatAdjustment"] >> target.m_combatAdjustment;
    map["attackAdjustment"] >> target.m_attackAdjustment;
}

}}
