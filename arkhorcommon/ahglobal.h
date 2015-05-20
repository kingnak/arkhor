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
        AllPhases = Upkeep | Movement | ArkhamEncountery | OtherWorldEncountery | Mythos
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
        Obj_Blessing,
        Obj_Curse
    };

    enum Dimension {
        NoDimension,
        Dim_Square,
        Dim_Circle,
        Dim_Moon,
        Dim_Bar
    };

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
