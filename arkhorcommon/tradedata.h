#ifndef TRADEDATA_H
#define TRADEDATA_H

#include <serializer.hpp>
#include <ahglobal.h>

namespace AH {
    namespace Common {
        struct ARKHOR_COMMON_EXPORTS TradeData
        {
            TradeData() : phase(NoOffer) {}

            enum TradePhase {
                NoOffer,
                InitialOffer,
                CounterOffer,
                AcceptOffer
            };

            TradePhase phase;
            QString originChar;
            QStringList originOffer;
            QString destChar;
            QStringList destOffer;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, TradeData);
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, TradeData::TradePhase);
    }
}
#endif // TRADEDATA_H
