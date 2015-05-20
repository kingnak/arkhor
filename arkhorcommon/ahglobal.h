#ifndef AHGLOBAL_H
#define AHGLOBAL_H

#include "arkhorcommonglobal.h"
#include "serializer.hpp"
#include <QFlag>

namespace AH
{
    enum Attribute {
        NoAttribute,
        Attr_Speed,
        Attr_Sneak,
        Attr_Fight,
        Attr_Will,
        Attr_Lore,
        Attr_Luck
    };

    enum GamePhase {
        NoGamePhase = 0x0000,
        Upkeep = 0x0001,
        Movement = 0x0002,
        ArkhamEncountery = 0x0004,
        OtherWorldEncountery = 0x0008,
        Mythos = 0x0010,
        AllPhases = Upkeep | Movement | ArkhamEncountery | OtherWorldEncountery | Mythos,

        // Special Phases
        DieRoll = 0x0100,
        FightPhase = 0x0200,
    };

    Q_DECLARE_FLAGS(GamePhases, GamePhase)
    Q_DECLARE_OPERATORS_FOR_FLAGS(GamePhases)

    enum Skill {
        NoSkill,
        Skill_Speed,
        Skill_Sneak,
        Skill_Fight,
        Skill_Will,
        Skill_Lore,
        Skill_Luck,
        Skill_Evade,
        Skill_Combat,
        Skill_Horror,
        Skill_Spell
    };

    ARKHOR_COMMON_EXPORTS Attribute attributeForSkillTest(Skill skill);

    enum GameObjectType {
        NoObject,
        Obj_CommonItem,
        Obj_UniqueItem,
        Obj_Spell,
        Obj_Skill,
        Obj_Ally,
        Obj_Blessing_Curse,
        Obj_Special
    };

    enum Dimension {
        NoDimension = 0,
        Dim_Square = 0x0001,
        Dim_Circle = 0x0002,
        Dim_Moon = 0x0004,
        Dim_Slash = 0x0008,
        Dim_Star = 0x0010,
        Dim_Diamond = 0x0020,
        Dim_Hexagon = 0x0040,
        Dim_Plus = 0x0080,
        Dim_Triangle = 0x0100,
    };
    Q_DECLARE_FLAGS(Dimensions, Dimension)

    enum OtherWorldPhase {
        NoPhase,
        OWP_FirstField,
        OWP_SecondField
    };

    enum ContinueType {
        CanContinue,
        CannotContinue
    };

    enum ChooseType {
        ChooseMandatory,
        ChooseOptional,
        ChooseSupplemental
    };

    ARKHOR_COMMON_EXPORTS void registerCommonMetaTypes();

    DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, AH::Attribute);
    DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, AH::GamePhase);
    DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, AH::GamePhases);
    DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, AH::Skill);
    DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, AH::GameObjectType);
    DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, AH::Dimension);
    DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, AH::OtherWorldPhase);
    DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, AH::ContinueType);
    DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, AH::ChooseType);
}

#endif // AHGLOBAL_H
