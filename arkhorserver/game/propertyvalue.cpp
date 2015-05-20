#include "propertyvalue.h"

bool PropertyValue::isAttributeProperty(PropertyValue::Property p)
{
    return (p & ATTR_OFFSET) == ATTR_OFFSET;
}

bool PropertyValue::isSkillProperty(PropertyValue::Property p)
{
    return (p & SKILL_OFFSET) == SKILL_OFFSET;
}

bool PropertyValue::isCharacterProperty(PropertyValue::Property p)
{
    return (p & PROP_OFFSET) == PROP_OFFSET;
}

bool PropertyValue::isGameProperty(PropertyValue::Property p)
{
    return (p & GAME_OFFSET) == GAME_OFFSET;
}

bool PropertyValue::isMonsterProperty(PropertyValue::Property p)
{
    return (p & MONSTER_OFFSET) == MONSTER_OFFSET;
}

PropertyValue::Property PropertyValue::attribute2Property(AH::Attribute attr)
{
    switch (attr) {
    case AH::NoAttribute:
        return NoProperty;
    case AH::Attr_Speed:
    case AH::Attr_Sneak:
    case AH::Attr_Fight:
    case AH::Attr_Will:
    case AH::Attr_Lore:
    case AH::Attr_Luck:
    default:
        return static_cast<Property> ( ATTR_OFFSET | attr );
    }
}

PropertyValue::Property PropertyValue::skill2Property(AH::Skill skill)
{
    switch (skill) {
    case AH::NoSkill:
        return NoProperty;
    case AH::Skill_Speed:
    case AH::Skill_Sneak:
    case AH::Skill_Fight:
    case AH::Skill_Will:
    case AH::Skill_Lore:
    case AH::Skill_Luck:
    case AH::Skill_Evade:
    case AH::Skill_Combat:
    case AH::Skill_Horror:
    case AH::Skill_Spell:
    default:
        return static_cast<Property> ( SKILL_OFFSET | skill );
    }
}

AH::Skill PropertyValue::property2Skill(PropertyValue::Property prop)
{
    if (isSkillProperty(prop)) {
        return static_cast<AH::Skill> (prop & (~SKILL_OFFSET));
    }
    return AH::NoSkill;
}

AH::Attribute PropertyValue::property2Attribute(PropertyValue::Property prop)
{
    if (isAttributeProperty(prop)) {
        return static_cast<AH::Attribute> (prop & (~ATTR_OFFSET));
    }
    return AH::NoAttribute;
}

AH::Common::PropertyValueData::Property PropertyValue::skill2DieRoll(AH::Skill skill)
{
    return static_cast<PropertyValue::Property> (DIEROLL_OFFSET | skill);
}
