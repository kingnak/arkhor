#ifndef PROPERTYVALUEDATA_H
#define PROPERTYVALUEDATA_H

#include "arkhorcommonglobal.h"
#include "serializer.hpp"
#include "ahglobal.h"
#include <QStringList>

namespace AH {
    namespace Common {

        class ARKHOR_COMMON_EXPORTS PropertyValueData
        {
        protected:
            enum {
                ATTR_OFFSET = 0x100,
                SKILL_OFFSET = 0x200,
                PROP_OFFSET = 0x400,
                DIEROLL_OFFSET = 0x800,
                GAME_OFFSET = 0x1000,
                DAMAGE_OFFSET = 0x2000
            };
        public:
            enum Property {
                NoProperty = 0,

                Attr_Speed = ATTR_OFFSET | AH::Attr_Speed,
                Attr_Sneak = ATTR_OFFSET | AH::Attr_Sneak,
                Attr_Fight = ATTR_OFFSET | AH::Attr_Fight,
                Attr_Will = ATTR_OFFSET | AH::Attr_Will,
                Attr_Lore = ATTR_OFFSET | AH::Attr_Lore,
                Attr_Luck = ATTR_OFFSET | AH::Attr_Luck,

                Skill_Speed = SKILL_OFFSET | AH::Skill_Speed,
                Skill_Sneak = SKILL_OFFSET | AH::Skill_Sneak,
                Skill_Fight = SKILL_OFFSET | AH::Skill_Fight,
                Skill_Will = SKILL_OFFSET | AH::Skill_Will,
                Skill_Lore = SKILL_OFFSET | AH::Skill_Lore,
                Skill_Luck = SKILL_OFFSET | AH::Skill_Luck,
                Skill_Evade = SKILL_OFFSET | AH::Skill_Evade,
                Skill_Combat = SKILL_OFFSET | AH::Skill_Combat,
                Skill_Horror = SKILL_OFFSET | AH::Skill_Horror,
                Skill_Spell = SKILL_OFFSET | AH::Skill_Spell,

                Prop_MaxStamina = PROP_OFFSET,
                Prop_MaxSanity,
                Prop_Focus,
                Prop_Movement,
                Prop_MinSuccessDieRoll,
                Prop_HandCount,

                DieRoll_All = DIEROLL_OFFSET | AH::NoSkill,
                DieRoll_Speed = DIEROLL_OFFSET | AH::Skill_Speed,
                DieRoll_Sneak = DIEROLL_OFFSET | AH::Skill_Sneak,
                DieRoll_Fight = DIEROLL_OFFSET | AH::Skill_Fight,
                DieRoll_Will = DIEROLL_OFFSET | AH::Skill_Will,
                DieRoll_Lore = DIEROLL_OFFSET | AH::Skill_Lore,
                DieRoll_Luck = DIEROLL_OFFSET | AH::Skill_Luck,
                DieRoll_Evade = DIEROLL_OFFSET | AH::Skill_Evade,
                DieRoll_Combat = DIEROLL_OFFSET | AH::Skill_Combat,
                DieRoll_Horror = DIEROLL_OFFSET | AH::Skill_Horror,
                DieRoll_Spell = DIEROLL_OFFSET | AH::Skill_Spell,

                Game_SealClueCost = GAME_OFFSET,

                Damage_General = DAMAGE_OFFSET,
                Damage_Magical,
                Damage_Physical
            };

            PropertyValueData(Property prop = NoProperty, int val = 0)
                : m_val(val), m_prop(prop) {}
            virtual ~PropertyValueData() {}

            int value() const { return m_val; }
            Property property() const { return m_prop; }

        protected:
            int m_val;
            Property m_prop;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, PropertyValueData);
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, PropertyValueData::Property);

        class ARKHOR_COMMON_EXPORTS PropertyModificationData
        {
        public:
            PropertyModificationData(PropertyValueData::Property prop = PropertyValueData::NoProperty, int modification = 0)
                : m_prop(prop), m_mod(modification) {}
            virtual ~PropertyModificationData() {}

            PropertyValueData::Property affectedProperty() const { return m_prop; }
            int modificationAmount() const { return m_mod; }
            virtual QString modifierId() const { return m_modifierId; }

        protected:
            PropertyValueData::Property m_prop;
            int m_mod;
            QString m_modifierId;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, PropertyModificationData);
        };

        class ARKHOR_COMMON_EXPORTS ModifiedPropertyValueData
        {
        public:
            ModifiedPropertyValueData()
                : m_finalVal(0) {}
            ModifiedPropertyValueData(PropertyValueData prop, int finVal, QList<PropertyModificationData> mods)
                : m_property(prop), m_finalVal(finVal), m_mods(mods) {}

            PropertyValueData property() const { return m_property; }
            int finalVal() const { return m_finalVal; }
            QList<PropertyModificationData> modifications() const { return m_mods; }

        protected:
            PropertyValueData m_property;
            int m_finalVal;
            QList<PropertyModificationData> m_mods;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, ModifiedPropertyValueData);
        };
    }
}

#endif // PROPERTYVALUEDATA_H
