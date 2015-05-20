#include "propertyvaluedata.h"

namespace AH { namespace Common {

DEFINE_ENUM_SERIALIZER(PropertyValueData::Property)

void operator <<(QVariant &data, const PropertyValueData &target)
{
    QVariantMap map;
    map["property"] << target.property();
    map["value"] << target.value();
    data << map;
}

void operator >>(const QVariant &data, PropertyValueData &target)
{
    QVariantMap map;
    data >> map;
    map["property"] >> target.m_prop;
    map["value"] >> target.m_val;
}

}}
