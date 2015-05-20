#include "investigatordata.h"

namespace AH{ namespace Common {

InvestigatorData::InvestigatorData()
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
    map["attrSpeedSneak"] << c.m_attrSpeedSneak;
    map["attrFightWill"] << c.m_attrFightWill;
    map["attrLoreLuck"] << c.m_attrLoreLuck;
    map["uniqueAbilityId"] << c.m_uniqueAbilityId;
    map["fixedPossesionObjectIds"] << c.m_fixedPossesionObjectIds;
    map["randomPossesions"] << c.m_randomPossessions;
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
    map["attrSpeedSneak"] >> c.m_attrSpeedSneak;
    map["attrFightWill"] >> c.m_attrFightWill;
    map["attrLoreLuck"] >> c.m_attrLoreLuck;
    map["uniqueAbilityId"] >> c.m_uniqueAbilityId;
    map["fixedPossesionObjectIds"] >> c.m_fixedPossesionObjectIds;
    map["randomPossesions"] >> c.m_randomPossessions;
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

void operator <<(QVariant &data, const InvestigatorData::ObjectTypeCount &c) {
    QVariantMap map;
    map["type"] << c.type;
    map["amount"] << c.amount;
    data << map;
}

void operator >>(const QVariant &data, InvestigatorData::ObjectTypeCount &c) {
    QVariantMap map;
    data >> map;
    map["type"] >> c.type;
    map["amount"] >> c.amount;
}

}}
