#ifndef MONSTERDATA_H
#define MONSTERDATA_H

#include <arkhorcommonglobal.h>
#include <serializer.hpp>
#include <ahglobal.h>
#include <fielddata.h>

namespace AH {
    namespace Common {

        class MonsterData
        {
        public:
            MonsterData();
            virtual ~MonsterData() {}

            enum MovementType {
                Normal,
                Fast,
                Stationary,
                Flying,
                Special
            };

            enum MonsterAttribute {
                Ambush = 0x0001,
                Endless = 0x0002,
                PhysicalResistance = 0x0004,
                MagicalResistance = 0x0008,
                PhysicalImmunity = 0x0010,
                MagicalImmunity = 0x0020,
                Nightmarish_1 = 0x0100,
                Nightmarish_2 = 0x0200,
                Nightmarish_3 = 0x0400,
                Overwhelming_1 = 0x1000,
                Overwhelming_2 = 0x2000,
                Overwhelming_3 = 0x4000,
            };
            Q_DECLARE_FLAGS(MonsterAttributes, MonsterAttribute)

            virtual MonsterData *data() { return this; }

            virtual QString id() const { return m_id; }
            virtual QString name() const { return m_name; }
            virtual QString description() const { return m_desc; }
            virtual AH::Dimension dimension() const { return m_dimension; }
            virtual MovementType movementType() const { return m_movement; }
            virtual int awareness() const { return m_awareness; }
            virtual int horrorAdjustment() const { return m_horrorAdjustment; }
            virtual int horrorDamage() const { return m_horrorDamage; }
            virtual int combatAdjustment() const { return m_combatAdjustment; }
            virtual int combatValue() const { return m_combatValue; }
            virtual int toughness() const { return m_toughness; }
            virtual MonsterAttributes attributes() const { return m_attributes; }
            virtual AH::Common::FieldData::FieldID fieldId() const { return m_fieldId; }

        protected:
            QString m_id;
            QString m_name;
            QString m_desc;
            AH::Dimension m_dimension;
            MovementType m_movement;
            int m_awareness;
            int m_horrorAdjustment;
            int m_horrorDamage;
            int m_combatAdjustment;
            int m_combatValue;
            int m_toughness;
            MonsterAttributes m_attributes;
            FieldData::FieldID m_fieldId;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, MonsterData);
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, MonsterData::MovementType);
        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, MonsterData::MonsterAttribute);
        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, MonsterData::MonsterAttributes);

    }

}

#endif // MONSTERDATA_H
