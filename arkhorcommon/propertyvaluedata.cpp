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

void operator <<(QVariant &data, const PropertyModificationData &target)
{
    QVariantMap map;
    map["property"] << target.m_prop;
    map["modification"] << target.m_mod;
    map["modifierId"] << target.m_modifierId;
    data << map;
}

void operator >>(const QVariant &data, PropertyModificationData &target)
{
   QVariantMap map;
   data >> map;
   map["property"] >> target.m_prop;
   map["modification"] >> target.m_mod;
   map["modifierId"] >> target.m_modifierId;
}

void operator <<(QVariant &data, const ModifiedPropertyValueData &target)
{
    QVariantMap map;
    map["property"] << target.m_property;
    map["finalValue"] << target.m_finalVal;
    map["modificationList"] << target.m_mods;
    data << map;
}

void operator >>(const QVariant &data, ModifiedPropertyValueData &target)
{
   QVariantMap map;
   data >> map;
   map["property"] >> target.m_property;
   map["finalValue"] >> target.m_finalVal;
   map["modificationList"] >> target.m_mods;
}
}}
