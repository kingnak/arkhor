#ifndef PROPERTYVALUEDATA_H
#define PROPERTYVALUEDATA_H

#include "arkhorcommonglobal.h"
#include "serializer.hpp"
#include "ahglobal.h"

namespace AH {
    namespace Common {

        class PropertyValueData
        {
        protected:
            enum {
                ATTR_OFFSET = 0x100,
                SKILL_OFFSET = 0x200,
                PROP_OFFSET = 0x400,

                GAME_OFFSET = 0x1000
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

                Game_SealClueCost = GAME_OFFSET
            };

            PropertyValueData(Property prop, int val)
                : m_val(val), m_prop(prop) {}

            int value() const { return m_val; }
            Property property() const { return m_prop; }

        protected:
            int m_val;
            Property m_prop;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, PropertyValueData);
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, PropertyValueData::Property);
    }
}

#endif // PROPERTYVALUEDATA_H
