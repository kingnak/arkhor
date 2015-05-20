#include "attributesliderdata.h"

namespace AH { namespace Common {

void operator <<(QVariant &data, const AttributeValue &target)
{
    QVariantMap map;
    map["attr"] << target.attribute;
    map["value"] << target.value;
    data << map;
}

void operator >>(const QVariant &data, AttributeValue &target)
{
    QVariantMap map;
    data >> map;
    map["attr"] >> target.attribute;
    map["value"] >> target.value;
}

void operator <<(QVariant &data, const AttributePair &target)
{
    QVariantMap map;
    map["first"] << target.m_first;
    map["second"] << target.m_second;
    data << map;
}

void operator >>(const QVariant &data, AttributePair &target)
{
    QVariantMap map;
    data >> map;
    map["first"] >> target.m_first;
    map["second"] >> target.m_second;
}

void operator <<(QVariant &data, const AttributeSliderData &target)
{
    QVariantMap map;
    map["availableSettings"] << target.m_attributeSettings;
    map["curPos"] << target.m_curSettingPos;
    data << map;
}

void operator >>(const QVariant &data, AttributeSliderData &target)
{
    QVariantMap map;
    data >> map;
    map["availableSettings"] >> target.m_attributeSettings;
    map["curPos"] >> target.m_curSettingPos;
}

}}
