#ifndef DIEDATA_H
#define DIEDATA_H

#include <ahglobal.h>
#include "arkhorcommonglobal.h"
#include <serializer.hpp>
#include <QStringList>
#include <QPair>
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
                : m_type(None), m_skill(NoSkill) {}

            DieRollData(RollType type)
                : m_type(type), m_skill(NoSkill)  {}

            RollType type() const { return m_type; }
            Skill skill() const { return m_skill; }
            DiePoolData pool() const { return m_pool; }
            DiePoolData &pool() { return m_pool; }
            QList<quint32> successRolls() const { return m_successes; }

            void setType(RollType type) { m_type = type; }
            void setSkill(Skill skill) { m_skill = skill; }
            void setPool(const DiePoolData &pool) { m_pool = pool; }
            void setSuccessRolls(QList<quint32> successes) { m_successes = successes; }

        protected:
            RollType m_type;
            Skill m_skill;
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

            struct OptionDescription {
                QString id;
                QString desc;
                QString sourceId;
            };

            DieRollTestData()
                : m_type(None), m_target(0), m_diceForClue(0), m_succeeded(false) {}

            DieRollTestData(TestType type, int target = 0)
                : m_type(type), m_target(target), m_diceForClue(0), m_succeeded(false) {}

            TestType type() const { return m_type; }
            QString description() const { return m_desc; }
            QString sourceId() const { return m_sourceId; }
            DieRollData rollData() const { return m_roller; }
            DieRollData &rollData() { return m_roller; }
            int targetValue() const { return m_target; }
            int diceForClueBurn() const { return m_diceForClue; }
            QList<OptionDescription> dieRollOptions() const { return m_options; }
            ModifiedPropertyValueData clueBurnMods() const { return m_clueBurnMods; }
            bool isSucceeded() const { return m_succeeded; }
            QList<PropertyModificationData> generalModifications() const { return m_generalMods; }

            void setType(TestType type) { m_type = type; }
            void setDescription(const QString &desc) { m_desc = desc; }
            void setSourceId(const QString &sourceId) { m_sourceId = sourceId; }
            void setRollData(const DieRollData &drd) { m_roller = drd; }
            void setDiceForClueBurn(int d4cb) { m_diceForClue = d4cb; }
            void setRollOptions(QList<OptionDescription> lst) { m_options = lst; }
            void setClueBurnMods(ModifiedPropertyValueData mods) { m_clueBurnMods = mods; }
            void setSucceeded(bool success) { m_succeeded = success; }
            void setTargetValue(int val) { m_target = val; }
            void addGeneralModifications(QList<PropertyModificationData> mods) { m_generalMods << mods; }

        protected:
            TestType m_type;
            DieRollData m_roller;
            int m_target;
            int m_diceForClue;
            QList<OptionDescription> m_options;
            ModifiedPropertyValueData m_clueBurnMods;
            QList<PropertyModificationData> m_generalMods;
            QString m_desc;
            QString m_sourceId;
            bool m_succeeded;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, DieRollTestData);
            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, DieRollTestData::OptionDescription);
        };
        //Q_DECLARE_METATYPE(DieRollTestData::OptionDescription)

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
