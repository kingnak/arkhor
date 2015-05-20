#include "choicedata.h"

namespace AH { namespace Common {

DEFINE_ENUM_SERIALIZER(ChoiceData::ChoiceType)

void ChoiceData::setSelectObjects(QStringList objectIds, int min, int max)
{
    m_type = ChooseObjects;
    m_min = min;
    m_max = max;
    m_objectIds = objectIds;
}

void ChoiceData::setSelectPayment(Cost c)
{
    m_type = ChoosePayment;
    m_costs = c;
}

void ChoiceData::setSelectStrings(QList<ChoiceData::OptionData> options)
{
    m_type = ChooseString;
    m_strings = options;
}

void operator <<(QVariant &data, const ChoiceData &target)
{
    QVariantMap map;
    map["type"] << target.m_type;
    map["description"] << target.m_desc;
    map["min"] << target.m_min;
    map["max"] << target.m_max;
    map["objectIds"] << target.m_objectIds;
    map["costs"] << target.m_costs;
    map["strings"] << target.m_strings;
    data << map;
}

void operator >>(const QVariant &data, ChoiceData &target)
{
    QVariantMap map;
    data >> map;
    map["type"] >> target.m_type;
    map["description"] >> target.m_desc;
    map["min"] >> target.m_min;
    map["max"] >> target.m_max;
    map["objectIds"] >> target.m_objectIds;
    map["costs"] >> target.m_costs;
    map["strings"] >> target.m_strings;
}

void operator <<(QVariant &data, const ChoiceData::OptionData &target)
{
    QVariantMap map;
    map["id"] << target.id;
    map["name"] << target.name;
    map["description"] << target.description;
    data << map;
}

void operator >>(const QVariant &data, ChoiceData::OptionData &target)
{
    QVariantMap map;
    data >> map;
    map["id"] >> target.id;
    map["name"] >> target.name;
    map["description"] >> target.description;
}

}}
