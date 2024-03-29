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

bool PropertyValue::isDrawCardProperty(AH::Common::PropertyValueData::Property p)
{
    return (p & DRAWCARD_OFFSET) == DRAWCARD_OFFSET;
}

bool PropertyValue::isIgnoreProperty(AH::Common::PropertyValueData::Property p)
{
    return (p & IGNORE_OFFSET) == IGNORE_OFFSET;
}

bool PropertyValue::isDieRollProperty(AH::Common::PropertyValueData::Property p)
{
    return (p & DIEROLL_OFFSET) == DIEROLL_OFFSET;
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

AH::Skill PropertyValue::dieRoll2Skill(AH::Common::PropertyValueData::Property prop)
{
    return static_cast<AH::Skill> (prop & ~DIEROLL_OFFSET);
}

AH::GameObjectType PropertyValue::property2ObjectType(AH::Common::PropertyValueData::Property prop)
{
    if (isDrawCardProperty(prop)) {
        // TODO: This will also affect Arkham and Other World Encounteries
        return static_cast<AH::GameObjectType> (prop & (~DRAWCARD_OFFSET));
    }
    return AH::NoObject;
}

AH::Common::PropertyValueData::Property PropertyValue::objectDraw2Property(AH::GameObjectType obj)
{
    return static_cast<PropertyValue::Property> (DRAWCARD_OFFSET | obj);
}
