#include "gamecontext.h"
#include "game.h"
#include "player.h"
#include "character.h"
#include "propertymodifier.h"

ModifiedPropertyValue GameContext::getCurCharacterProperty(PropertyValue::Property property)
{
    return getCharacterProperty(m_player->getCharacter(), property);
}

ModifiedPropertyValue GameContext::getCurCharacterSkill(AH::Skill skill)
{
    return getCharacterSkill(m_player->getCharacter(), skill);
}

ModifiedPropertyValue GameContext::getCurCharacterAttribute(AH::Attribute attr)
{
    return getCharacterAttribute(m_player->getCharacter(), attr);
}

ModifiedPropertyValue GameContext::getCharacterProperty(const Character *c, PropertyValue::Property property)
{
    AH::Attribute attr = PropertyValue::property2Attribute(property);
    if (attr != AH::NoAttribute)
        return getCharacterAttribute(c, attr);
    AH::Skill skill = PropertyValue::property2Skill(property);
    if (skill != AH::NoSkill)
        return getCharacterSkill(c, skill);

    Q_ASSERT(PropertyValue::isCharacterProperty(property));

    int base = 0;
    switch (property)
    {
    case PropertyValue::Prop_MaxStamina:
        base = c->maxStamina();
        break;
    case PropertyValue::Prop_MaxSanity:
        base = c->maxSanity();
        break;
    case PropertyValue::Prop_Focus:
        base = c->curFocus();
        break;
    case PropertyValue::Prop_Movement:
        base = c->movementPoints();
        break;
    default:
        Q_ASSERT_X(false, "GameContext::getCharacterProperty", "Property not defined");
    }

    PropertyModificationList mods = c->getPropertyModifiers().filtered(property);
    mods += m_game->getGameModifiers().filtered(property);

    int finalVal = mods.apply(base);

    return ModifiedPropertyValue(PropertyValue(property, base), finalVal, mods);
}

ModifiedPropertyValue GameContext::getCharacterSkill(const Character *c, AH::Skill skill)
{
    PropertyValue::Property prop = PropertyValue::skill2Property(skill);
    AH::Attribute attr = AH::attributeForSkillTest(skill);
    ModifiedPropertyValue attrValue = getCharacterAttribute(c, attr);
    PropertyModificationList mods = c->getPropertyModifiers().filtered(prop);
    mods += m_game->getGameModifiers().filtered(prop);

    int finalVal = mods.apply(attrValue.finalVal());

    ModifiedPropertyValue ret(
        PropertyValue(prop, attrValue.base()),
        finalVal,
        attrValue.modifiers() + mods
    );
    return ret;
}

ModifiedPropertyValue GameContext::getCharacterAttribute(const Character *c, AH::Attribute attr)
{
    PropertyValue::Property prop = PropertyValue::attribute2Property(attr);
    int base = c->getAttributeValue(attr);
    PropertyModificationList mods = c->getPropertyModifiers().filtered(prop);
    mods += m_game->getGameModifiers().filtered(prop);

    int finalVal = mods.apply(base);

    ModifiedPropertyValue ret(PropertyValue(prop, base), finalVal, mods);
    return ret;
}

ModifiedPropertyValue GameContext::getGameProperty(PropertyValue::Property property)
{
    Q_ASSERT(PropertyValue::isGameProperty(property));

    int base = 0;
    switch (property)
    {
    case PropertyValue::Game_SealClueCost:
        base = gGame->getGateSealClueCost();
        break;
    default:
        Q_ASSERT_X(false, "GameContext::getCharacterProperty", "Property not defined");
    }

    PropertyModificationList mods = gGame->getGameModifiers().filtered(property);

    int finalVal = mods.apply(base);

    ModifiedPropertyValue ret(PropertyValue(property, base), finalVal, mods);
    return ret;
}
