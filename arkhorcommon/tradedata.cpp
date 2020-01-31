#include "tradedata.h"

namespace AH { namespace Common {

DEFINE_ENUM_SERIALIZER(TradeData::TradePhase)

void operator <<(QVariant &data, const TradeData &target)
{
    QVariantMap map;
    map["phase"] << target.phase;
    map["originChar"] << target.originChar;
    map["originOffer"] << target.originOffer;
    map["destChar"] << target.destChar;
    map["destOffer"] << target.destOffer;
    data << map;
}

void operator >>(const QVariant &data, TradeData &target)
{
    QVariantMap map;
    data >> map;
    map["phase"] >> target.phase;
    map["originChar"] >> target.originChar;
    map["originOffer"] >> target.originOffer;
    map["destChar"] >> target.destChar;
    map["destOffer"] >> target.destOffer;
}

}}
