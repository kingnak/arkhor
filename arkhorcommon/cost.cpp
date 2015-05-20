#include "cost.h"

#ifdef ARKHOR_COMMON_EXPORTING
template class QList<AH::Common::CostItem>;
template class QSet<AH::Common::CostItem>;
template class QVector<AH::Common::CostItem>;
uint qHash(const AH::Common::CostItem &c) {
    return static_cast<uint> (c.type) | static_cast<uint> (c.amount << 4);
}
#endif

namespace AH { namespace Common {

DEFINE_ENUM_SERIALIZER(CostItem::PaymentItem)

Cost::Cost()
{
}

void operator <<(QVariant &data, const CostItem &target)
{
    QVariantMap map;
    map["type"] << target.type;
    map["amount"] << target.amount;
    data << map;
}

void operator >>(const QVariant &data, CostItem &target)
{
    QVariantMap map;
    data >> map;
    map["type"] >> target.type;
    map["amount"] >> target.amount;
}

void operator <<(QVariant &data, const CostList &target)
{
    QList<CostItem> lst = target;
    data << lst;
}

void operator >>(const QVariant &data, CostList &target)
{
    QList<CostItem> lst;
    data >> lst;
    target.clear();
    target += lst;
}

void operator <<(QVariant &data, const Cost &target)
{
    QVariantMap map;
    map["alternatives"] << target.m_alternatives;
    data << map;
}

void operator >>(const QVariant &data, Cost &target)
{
    QVariantMap map;
    data >> map;
    map["alternatives"] >> target.m_alternatives;
}

}}
