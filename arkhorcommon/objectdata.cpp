#include "objectdata.h"

namespace AH { namespace Common {

DEFINE_ENUM_SERIALIZER(RequestObjectsData::ObjectType)

void operator << (QVariant &data, const RequestObjectsData::ObjectRequest &target)
{
    QVariantMap map;
    map["type"] << target.first;
    map["id"] << target.second;
    data << map;
}

void operator >> (const QVariant &data, RequestObjectsData::ObjectRequest &target)
{
    QVariantMap map;
    data >> map;
    map["type"] >> target.first;
    map["id"] >> target.second;
}

void operator << (QVariant &data, const RequestObjectsData &target)
{
    QVariantMap map;
    map["requests"] << target.m_requests;
    data << map;
}

void operator >> (const QVariant &data, RequestObjectsData &target)
{
    QVariantMap map;
    data >> map;
    map["requests"] >> target.m_requests;
}

void operator << (QVariant &data, const DescribeObjectsData::ObjectDescription &target)
{
    QVariantMap map;
    map["type"] << target.first;
    map["description"] << target.second;
    data << map;
}

void operator >> (const QVariant &data, DescribeObjectsData::ObjectDescription &target)
{
    QVariantMap map;
    data >> map;
    map["type"] >> target.first;
    map["description"] >> target.second;
}

void operator << (QVariant &data, const DescribeObjectsData &target)
{
    QVariantMap map;
    map["descriptions"] << target.m_descs;
    data << map;
}

void operator >> (const QVariant &data, DescribeObjectsData &target)
{
    QVariantMap map;
    data >> map;
    map["descriptions"] >> target.m_descs;
}

}}
