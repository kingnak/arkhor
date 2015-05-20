#include "characterdata.h"

namespace AH { namespace Common {

CharacterData::CharacterData()
:   m_curStamina(0),
    m_curSanity(0),
    m_maxStamina(0),
    m_maxSanity(0),
    m_money(0),
    m_curFocus(0),
    m_movementPoints(0),
    m_clues(0),
    m_monsterToughness(0),
    m_fieldId(FieldData::NO_NO_FIELD),
    m_delayed(false),
    m_isSetOut(false),
    m_owPhase(AH::NoPhase)
{
}

void operator <<(QVariant &data, const CharacterData &c)
{
    QVariantMap map;
    map["id"] << c.id();
    map["investigatorId"] << c.investigatorId();
    map["investigator"] << c.investigatorData();
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
    map["monsterToughness"] << c.getMonsterToughness();
    map["fieldId"] << c.fieldId();
    map["delayed"] << c.isDelayed();
    map["isSetOut"] << c.isSetOut();
    map["otherWorldPhase"] << c.otherWorldPhase();
    data << map;
}

void operator >>(const QVariant &data, CharacterData &c)
{
    QVariantMap map;
    data >> map;
    map["id"] >> c.m_id;
    map["investigatorId"] >> c.m_investigatorId;
    map["investigator"] >> c.m_investigatorData;
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
    map["monsterToughness"] >> c.m_monsterToughness;
    map["fieldId"] >> c.m_fieldId;
    map["delayed"] >> c.m_delayed;
    map["isSetOut"] >> c.m_isSetOut;
    map["otherWorldPhase"] >> c.m_owPhase;
}

}}
