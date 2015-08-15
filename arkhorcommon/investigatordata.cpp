#include "investigatordata.h"

namespace AH{ namespace Common {

InvestigatorData::InvestigatorData()
:   m_homeFieldId(FieldData::NO_NO_FIELD),
    m_sanity(0),
    m_stamina(0),
    m_money(0),
    m_clues(0),
    m_focus(0)
{
}

void operator <<(QVariant &data, const InvestigatorData &c) {
    QVariantMap map;
    map["id"] << c.m_id;
    map["name"] << c.m_name;
    map["occupation"] << c.m_occupation;
    map["startFieldId"] << c.m_homeFieldId;
    map["sanity"] << c.m_sanity;
    map["stamina"] << c.m_stamina;
    map["money"] << c.m_money;
    map["focus"] << c.m_focus;
    map["clues"] << c.m_clues;
    map["attrSpeedSneak"] << c.m_attrSpeedSneak;
    map["attrFightWill"] << c.m_attrFightWill;
    map["attrLoreLuck"] << c.m_attrLoreLuck;
    map["uniqueAbilityId"] << c.m_uniqueAbilityId;
    map["fixedPossesionObjectIds"] << c.m_fixedPossesionObjectIds;
    map["randomPossesions"] << c.m_randomPossessions;
    map["uniqueAbilityName"] << c.uniqueAbilityName();
    map["uniqueAbilityDesc"] << c.uniqueAbilityDescription();
    map["fixedPossessionNames"] << c.fixedPossessionNames();
    data << map;
}

void operator >>(const QVariant &data, InvestigatorData &c) {
    QVariantMap map;
    data >> map;
    map["id"] >> c.m_id;
    map["name"] >> c.m_name;
    map["occupation"] >> c.m_occupation;
    map["startFieldId"] >> c.m_homeFieldId;
    map["sanity"] >> c.m_sanity;
    map["stamina"] >> c.m_stamina;
    map["money"] >> c.m_money;
    map["focus"] >> c.m_focus;
    map["clues"] >> c.m_clues;
    map["attrSpeedSneak"] >> c.m_attrSpeedSneak;
    map["attrFightWill"] >> c.m_attrFightWill;
    map["attrLoreLuck"] >> c.m_attrLoreLuck;
    map["uniqueAbilityId"] >> c.m_uniqueAbilityId;
    map["fixedPossesionObjectIds"] >> c.m_fixedPossesionObjectIds;
    map["randomPossesions"] >> c.m_randomPossessions;
    map["uniqueAbilityName"] >> c.m_uniqueAbilityName;
    map["uniqueAbilityDesc"] >> c.m_uniqueAbilityDesc;
    map["fixedPossessionNames"] >> c.m_fixedPossNames;
}

void operator <<(QVariant &data, const InvestigatorData::AttributeValuePair &c) {
    QVariantMap map;
    map["first"] << c.first;
    map["second"] << c.second;
    data << map;
}

void operator >>(const QVariant &data, InvestigatorData::AttributeValuePair &c) {
    QVariantMap map;
    data >> map;
    map["first"] >> c.first;
    map["second"] >> c.second;
}

}}
