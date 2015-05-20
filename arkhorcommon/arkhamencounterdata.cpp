#include "arkhamencounterdata.h"

namespace AH { namespace Common {

ArkhamEncounterData::ArkhamEncounterData()
    : m_fieldId(FieldData::NO_NO_FIELD)
{
}

void operator <<(QVariant &data, const ArkhamEncounterData &target)
{
    QVariantMap map;
    map["field"] << target.fieldId();
    map["options"] << target.optionData();
    map["description"] << target.description();
    data << map;
}

void operator >>(const QVariant &data, ArkhamEncounterData &target)
{
    QVariantMap map;
    data >> map;
    map["field"] >> target.m_fieldId;
    map["options"] >> target.m_optionData;
    map["description"] >> target.m_description;
}

}}
