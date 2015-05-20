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

void operator <<(QVariant &data, const ChoiceData &target)
{
    QVariantMap map;
    map["type"] << target.m_type;
    map["description"] << target.m_desc;
    map["min"] << target.m_min;
    map["max"] << target.m_max;
    map["objectIds"] << target.m_objectIds;
    map["costs"] << target.m_costs;
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
}

}}
