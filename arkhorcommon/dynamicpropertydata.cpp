#include "dynamicpropertydata.h"

namespace AH { namespace Common {

void operator <<(QVariant &data, const DynamicPropertyData &target)
{
    QVariantMap map;
    map["name"] << target.name();
    map["value"] << target.value();
    map["display"] << target.display();
    data << map;
}

void operator >>(const QVariant &data, DynamicPropertyData &target)
{
    QVariantMap map;
    data >> map;
    map["name"] >> target.m_name;
    map["value"] >> target.m_val;
    map["display"] >> target.m_display;
}

}}
