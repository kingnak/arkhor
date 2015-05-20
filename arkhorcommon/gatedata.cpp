#include "gatedata.h"

namespace AH { namespace Common {

GateData::GateData()
:   m_open(false),
    m_source(FieldData::NO_NO_FIELD),
    m_dest(FieldData::NO_NO_FIELD),
    m_dims(AH::NoDimension),
    m_adjustment(0)
{
}

void operator <<(QVariant &v, const GateData &target)
{
    QVariantMap map;
    map["id"] << target.id();
    map["isOpen"] << target.isOpen();
    map["source"] << target.source();
    map["destination"] << target.destination();
    map["dimensions"] << target.dimensions();
    map["closeAdjustment"] << target.closeAdjustment();
    v << map;
}

void operator >>(const QVariant &v, GateData &target)
{
    QVariantMap map;
    v >> map;
    map["id"] >> target.m_id;
    map["isOpen"] >> target.m_open;
    map["source"] >> target.m_source;
    map["destination"] >> target.m_dest;
    map["dimensions"] >> target.m_dims;
    map["closeAdjustment"] >> target.m_adjustment;
}

}}
