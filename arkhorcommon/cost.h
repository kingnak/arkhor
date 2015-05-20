#ifndef COST_H
#define COST_H

#include "arkhorcommonglobal.h"
#include <QList>
#include <QSet>
#include <QVector>
#include <serializer.hpp>

namespace AH {
    namespace Common {

        struct ARKHOR_COMMON_EXPORTS CostItem
        {
            enum PaymentItem {
                Pay_None,
                Pay_Money,
                Pay_Clue,
                Pay_Stamina,
                Pay_Sanity,
                Pay_Movement,
                Pay_GateTrophy,
                Pay_MonsterTrophy,
                Pay_MonsterStamina,

            };

            PaymentItem type;
            int amount;
            CostItem(PaymentItem type, int amount) : type(type), amount(amount) {}
            CostItem() : type(Pay_None), amount(0) {}

            bool operator == (const CostItem &o) const {
                return type == o.type && amount == o.amount;
            }

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, CostItem);
        };
        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, CostItem::PaymentItem);

        struct ARKHOR_COMMON_EXPORTS CostList : public QList<CostItem>
        {
            bool isValid() const {
                return !isEmpty();
            }

            static CostList invalid() {
                return CostList();
            }

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, CostList);
        };

        class ARKHOR_COMMON_EXPORTS Cost
        {
        public:
            Cost();

            void addAlternative(CostList l) { m_alternatives.append(l); }
            QList<CostList> getAlternatives() const { return m_alternatives; }
            void clear() { m_alternatives.clear(); }

        private:
            QList<CostList> m_alternatives;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, Cost);
        };

    }
}

uint ARKHOR_COMMON_EXPORTS qHash(const AH::Common::CostItem &);

#ifdef ARKHOR_COMMON_IMPORTING
extern template class QList<AH::Common::CostItem>;
extern template class QSet<AH::Common::CostItem>;
extern template class QVector<AH::Common::CostItem>;
#endif

#endif // COST_H
