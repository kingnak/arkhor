#include "utils.h"

using namespace AH::Common;

QString Utils::stringForProperty(AH::Common::PropertyValueData::Property p)
{
    switch (p) {
    case PropertyValueData::NoProperty: return "";

    case PropertyValueData::Attr_Speed: return "Speed";
    case PropertyValueData::Attr_Sneak: return "Sneak";
    case PropertyValueData::Attr_Fight: return "Fight";
    case PropertyValueData::Attr_Will: return "Will";
    case PropertyValueData::Attr_Lore: return "Lore";
    case PropertyValueData::Attr_Luck: return "Luck";

    case PropertyValueData::Skill_Speed: return "Speed";
    case PropertyValueData::Skill_Sneak: return "Sneak";
    case PropertyValueData::Skill_Fight: return "Fight";
    case PropertyValueData::Skill_Will: return "Will";
    case PropertyValueData::Skill_Lore: return "Lore";
    case PropertyValueData::Skill_Luck: return "Luck";
    case PropertyValueData::Skill_Evade: return "Evade";
    case PropertyValueData::Skill_Combat: return "Combat";
    case PropertyValueData::Skill_Horror: return "Horror";
    case PropertyValueData::Skill_Spell: return "Spell";

    case PropertyValueData::Prop_MaxStamina: return "Maximum Stamina";
    case PropertyValueData::Prop_MaxSanity: return "Maximum Sanity";
    case PropertyValueData::Prop_Focus: return "Focus";
    case PropertyValueData::Prop_Movement: return "Movement";
    case PropertyValueData::Prop_MinSuccessDieRoll: return "Minimal Success Die Roll";
    case PropertyValueData::Prop_HandCount: return "Hand Count";
    case PropertyValueData::Prop_CastCost: return "Spell Cast Sanity Cost";
    case PropertyValueData::Prop_CastAdjustment: return "Spell Cast Adjustment";

    case PropertyValueData::DieRoll_All: return "All";
    case PropertyValueData::DieRoll_Speed: return "Speed";
    case PropertyValueData::DieRoll_Sneak: return "Sneak";
    case PropertyValueData::DieRoll_Fight: return "Fight";
    case PropertyValueData::DieRoll_Will: return "Will";
    case PropertyValueData::DieRoll_Lore: return "Lore";
    case PropertyValueData::DieRoll_Luck: return "Luck";
    case PropertyValueData::DieRoll_Evade: return "Evade";
    case PropertyValueData::DieRoll_Combat: return "Combat";
    case PropertyValueData::DieRoll_Horror: return "Horror";
    case PropertyValueData::DieRoll_Spell: return "Spell";

    case PropertyValueData::Game_SealClueCost: return "Seal Clue Cost";

    case PropertyValueData::Damage_General: return "General Damage";
    case PropertyValueData::Damage_Magical: return "Magical Damage";
    case PropertyValueData::Damage_Physical: return "Physical Damage";
    }
    return "";
}

QString Utils::stringForDimension(AH::Dimension dim)
{
    switch (dim)
    {
    case AH::Dim_Square: return "Square";
    case AH::Dim_Circle: return "Circle";
    case AH::Dim_Moon: return "Moon";
    case AH::Dim_Slash: return "Slash";
    case AH::Dim_Star: return "Star";
    case AH::Dim_Diamond: return "Diamond";
    case AH::Dim_Hexagon: return "Hexagon";
    case AH::Dim_Plus: return "Plus";
    case AH::Dim_Triangle: return "Triangle";
    default:
        return "";
    }
}

QString Utils::stringForMovement(MonsterData::MovementType m)
{
    switch (m)
    {
    case AH::Common::MonsterData::Normal: return "Normal";
    case AH::Common::MonsterData::Fast: return "Fast";
    case AH::Common::MonsterData::Stationary: return "Stationary";
    case AH::Common::MonsterData::Flying: return "Flying";
    case AH::Common::MonsterData::Special: return "Special";
    }
    return "";
}

QString Utils::stringForMonsterAttribute(MonsterData::MonsterAttribute a)
{
    switch (a)
    {
    case AH::Common::MonsterData::Ambush: return "Ambush";
    case AH::Common::MonsterData::Endless: return "Endless";
    case AH::Common::MonsterData::PhysicalResistance: return "Physical Resistance";
    case AH::Common::MonsterData::MagicalResistance: return "Magical Resistance";
    case AH::Common::MonsterData::PhysicalImmunity: return "Physical Immunity";
    case AH::Common::MonsterData::MagicalImmunity: return "Magical Immunity";
    case AH::Common::MonsterData::Nightmarish_1: return "Nightmarish 1";
    case AH::Common::MonsterData::Nightmarish_2: return "Nightmarish 2";
    case AH::Common::MonsterData::Nightmarish_3: return "Nightmarish 3";
    case AH::Common::MonsterData::Overwhelming_1: return "Overwhelming 1";
    case AH::Common::MonsterData::Overwhelming_2: return "Overwhelming 2";
    case AH::Common::MonsterData::Overwhelming_3: return "Overwhelming 3";

    case AH::Common::MonsterData::MonsterAttribute_Max_Value_Sentinel:
        return "";
    }
    return "";
}

QStringList Utils::stringsForMonsterAttributes(MonsterData::MonsterAttributes attrs)
{
    QStringList l;
    for (int i = 1; i < AH::Common::MonsterData::MonsterAttribute_Max_Value_Sentinel; i <<= 1) {
        AH::Common::MonsterData::MonsterAttribute a = static_cast<AH::Common::MonsterData::MonsterAttribute> (i);
        if (attrs.testFlag(a)) {
            l << stringForMonsterAttribute(a);
        }
    }
    return l;
}
