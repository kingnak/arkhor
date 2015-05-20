#include "encounterdata.h"

namespace AH { namespace Common {

EncounterData::EncounterData()
    : m_fieldId(FieldData::NO_NO_FIELD)
{
}

void operator <<(QVariant &data, const EncounterData &target)
{
    QVariantMap map;
    map["field"] << target.fieldId();
    map["options"] << target.optionData();
    map["description"] << target.description();
    data << map;
}

void operator >>(const QVariant &data, EncounterData &target)
{
    QVariantMap map;
    data >> map;
    map["field"] >> target.m_fieldId;
    map["options"] >> target.m_optionData;
    map["description"] >> target.m_description;
}

}}
