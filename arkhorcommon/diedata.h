#ifndef DIEDATA_H
#define DIEDATA_H

#include <ahglobal.h>
#include "arkhorcommonglobal.h"
#include <serializer.hpp>
#include <QStringList>
#include "propertyvaluedata.h"

namespace AH {
    namespace Common {

        class ARKHOR_COMMON_EXPORTS DiePoolData
        {
        public:
            enum PoolType {
                None, Fixed, Property
            };

            DiePoolData(PoolType type = None)
                : m_type(type), m_count(0), m_adjust(0) {}

            DiePoolData(int count, int adjust)
                : m_type(Fixed), m_count(count), m_adjust(adjust) {}

            DiePoolData(ModifiedPropertyValueData prop, int adjust)
                : m_type(Property), m_count(prop.finalVal()), m_adjust(adjust), m_property(prop) {}

            PoolType type() const { return m_type; }
            int dieCount() const { return m_count; }
            int adjustment() const { return m_adjust; }
            ModifiedPropertyValueData property() const  { return m_property; }
            QList<quint32> dieValues() const { return m_dieVals; }

            void setDieCount(int count) { m_count = count; }
            void setAdjustment(int adjust) { m_adjust = adjust; }
            void setProperty(ModifiedPropertyValueData prop) { m_property = prop; }
            void setDieValues(QList<quint32> vals) { m_dieVals = vals; }

        protected:
            PoolType m_type;
            int m_count;
            int m_adjust;
            ModifiedPropertyValueData m_property;
            QList<quint32> m_dieVals;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, DiePoolData);
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, DiePoolData::PoolType);

        class ARKHOR_COMMON_EXPORTS DieRollData
        {
        public:
            enum RollType {
                None, Sum, Count
            };

            DieRollData()
                : m_type(None) {}

            DieRollData(RollType type)
                : m_type(type) {}

            RollType type() const { return m_type; }
            DiePoolData pool() const { return m_pool; }
            DiePoolData &pool() { return m_pool; }
            QList<quint32> successRolls() const { return m_successes; }

            void setPool(const DiePoolData &pool) { m_pool = pool; }
            void setSuccessRolls(QList<quint32> successes) { m_successes = successes; }

        protected:
            RollType m_type;
            DiePoolData m_pool;
            QList<quint32> m_successes;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, DieRollData);
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, DieRollData::RollType);

        class ARKHOR_COMMON_EXPORTS DieRollTestData
        {
        public:
            enum TestType {
                None, Value, Boolean
            };

            DieRollTestData()
                : m_type(None), m_target(0) {}

            DieRollTestData(TestType type, int target = 0)
                : m_type(type), m_target(target) {}

            TestType type() const { return m_type; }
            DieRollData rollData() const { return m_roller; }
            DieRollData &rollData() { return m_roller; }
            int targetValue() const { return m_target; }
            int diceForClueBurn() const { return m_diceForClue; }
            QStringList dieRollOptionIds() const { return m_optionIds; }

            void setRollData(const DieRollData &drd) { m_roller = drd; }
            void setDiceForClueBurn(int d4cb) { m_diceForClue = d4cb; }
            void setRollOptionIds(QStringList lst) { m_optionIds = lst; }

        protected:
            TestType m_type;
            DieRollData m_roller;
            int m_target;
            int m_diceForClue;
            QStringList m_optionIds;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, DieRollTestData);
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, DieRollTestData::TestType);

        class DieTestUpdateData
        {
        public:
            DieTestUpdateData() {}

            DieTestUpdateData(QString optId, int clueBurns = 0)
                : m_optionId(optId), m_clueBurnAmount(clueBurns)
            {}

            QString dieRollOptionId() const { return m_optionId; }
            int clueBurnAmount() const { return m_clueBurnAmount; }

        protected:
            QString m_optionId;
            int m_clueBurnAmount;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, DieTestUpdateData);
        };

    }
}

#endif // DIEDATA_H
