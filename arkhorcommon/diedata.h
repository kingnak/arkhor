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

            DiePoolData()
                : m_type(None), m_count(0), m_adjust(0) {}

            DiePoolData(int count, int adjust)
                : m_type(Fixed), m_count(count), m_adjust(adjust) {}

            DiePoolData(ModifiedPropertyValueData prop, int adjust)
                : m_type(Property), m_count(0), m_adjust(adjust), m_property(prop) {}

            PoolType type() const { return m_type; }
            int dieCount() const { return m_count; }
            int adjustment() const { return m_adjust; }
            ModifiedPropertyValueData property() const  { return m_property; }
            QList<quint32> dieValues() const { return m_dieVals; }
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

            DieRollData(DiePoolData pool)
                : m_type(Sum), m_pool(pool) {}

            DieRollData(DiePoolData pool, QList<quint32> successes)
                : m_type(Count), m_pool(pool), m_successes(successes) {}

            RollType type() const { return m_type; }
            DiePoolData pool() const { return m_pool; }
            QList<quint32> successRolls() const { return m_successes; }

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

            DieRollTestData(DieRollData roller)
                : m_type(Value), m_roller(roller), m_target(0) {}

            DieRollTestData(DieRollData roller, int target)
                : m_type(Boolean), m_roller(roller), m_target(target) {}

            TestType type() const { return m_type; }
            DieRollData rollData() const { return m_roller; }
            int targetValue() const { return m_target; }

        protected:
            TestType m_type;
            DieRollData m_roller;
            int m_target;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, DieRollTestData);
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, DieRollTestData::TestType);
    }
}

#endif // DIEDATA_H
