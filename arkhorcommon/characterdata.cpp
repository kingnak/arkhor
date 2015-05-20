#include "characterdata.h"

namespace AH { namespace Common {

CharacterData::CharacterData()
{
}

void operator <<(QVariant &data, const CharacterData &c)
{
    QVariantMap map;
    map["investigatorId"] << c.investigatorId();
    map["curStamina"] << c.curStamina();
    map["curSanity"] << c.curSanity();
    map["maxStamina"] << c.maxStamina();
    map["maxSanity"] << c.maxSanity();
    map["money"] << c.money();
    map["curFocus"] << c.curFocus();
    map["movementPoints"] << c.movementPoints();
    map["clues"] << c.clues();
    map["attrSettings"] << c.attrSettings();
    map["inventoryIds"] << c.inventoryIds();
    map["monsterMarkerIds"] << c.monsterMarkerIds();
    map["gateMarkerIds"] << c.gateMarkerIds();
    map["fieldId"] << c.fieldId();
    map["delayed"] << c.isDelayed();
    map["otherWorldPhase"] << c.otherWorldPhase();
    data << map;
}

void operator >>(const QVariant &data, CharacterData &c)
{
    QVariantMap map;
    data >> map;
    map["investigatorId"] >> c.m_investigatorId;
    map["curStamina"] >> c.m_curStamina;
    map["curSanity"] >> c.m_curSanity;
    map["maxStamina"] >> c.m_maxStamina;
    map["maxSanity"] >> c.m_maxSanity;
    map["money"] >> c.m_money;
    map["curFocus"] >> c.m_curFocus;
    map["movementPoints"] >> c.m_movementPoints;
    map["clues"] >> c.m_clues;
    map["attrSettings"] >> c.m_attrSettings;
    map["inventoryIds"] >> c.m_inventoryIds;
    map["monsterMarkerIds"] >> c.m_monsterMarkerIds;
    map["gateMarkerIds"] >> c.m_gateMarkerIds;
    map["fieldId"] >> c.m_fieldId;
    map["delayed"] >> c.m_delayed;
    map["otherWorldPhase"] >> c.m_owPhase;
}

}}
