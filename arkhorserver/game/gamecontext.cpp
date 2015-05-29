#include "gamecontext.h"
#include "game.h"
#include "player.h"
#include "character.h"
#include "monster.h"
#include "propertymodifier.h"
#include "ancientone.h"

AncientOne *GameContext::ancientOne()
{
    return gGame->ancientOne();
}

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

ModifiedPropertyValue GameContext::getCurCharacterClueBurn(AH::Skill skill)
{
    return getCharacterClueBurn(m_player->getCharacter(), skill);
}

ModifiedPropertyValue GameContext::getCurCharacterDrawObject(AH::GameObjectType type)
{
    return getCharacterDrawObject(m_player->getCharacter(), type);
}

bool GameContext::checkCurCharacterIgnoreMonsterAttribute(AH::Common::MonsterData::MonsterAttribute attr)
{
    return checkCharacterIgnoreMonsterAttribute(m_player->getCharacter(), attr);
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
    case PropertyValue::Prop_MinSuccessDieRoll:
        // TODO Blessing/Curse ==> No, that are objects ==> handled by modifiers below
        base = 5;
        break;
    case PropertyValue::Prop_HandCount:
        base = 2;
        break;

    default:
        if (PropertyValue::isDrawCardProperty(property)) {
            base = 1;
            break;
        }

        if (PropertyValue::isIgnoreProperty(property)) {
            base = 0;
            break;
        }

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

    // Special skills also use base skill modifiers!
    AH::Skill baseSkill = AH::baseSkillForSpecialSkill(skill);
    if (baseSkill != AH::NoSkill) {
        PropertyValue::Property baseProp = PropertyValue::skill2Property(baseSkill);
        mods += c->getPropertyModifiers().filtered(baseProp);
        mods += m_game->getGameModifiers().filtered(baseProp);
    }

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

ModifiedPropertyValue GameContext::getCharacterClueBurn(const Character *c, AH::Skill skill)
{
    PropertyValue::Property prop = PropertyValue::skill2DieRoll(skill);
    PropertyModificationList mods = c->getPropertyModifiers().filtered(prop);
    PropertyValue::Property propAll = static_cast<PropertyValue::Property> (PropertyValue::DieRoll_All);
    mods += c->getPropertyModifiers().filtered(propAll);

    // Special skills also use base skill modifiers!
    AH::Skill baseSkill = AH::baseSkillForSpecialSkill(skill);
    if (baseSkill != AH::NoSkill) {
        PropertyValue::Property baseProp = PropertyValue::skill2DieRoll(baseSkill);
        mods += c->getPropertyModifiers().filtered(baseProp);
        mods += m_game->getGameModifiers().filtered(baseProp);
    }

    int base = 1;

    int finalVal = mods.apply(base);

    ModifiedPropertyValue ret(PropertyValue(prop, base), finalVal, mods);
    return ret;
}

ModifiedPropertyValue GameContext::getCharacterDrawObject(const Character *c, AH::GameObjectType type)
{
    PropertyValue::Property prop = PropertyValue::objectDraw2Property(type);
    PropertyModificationList mods = c->getPropertyModifiers().filtered(prop);
    mods += m_game->getGameModifiers().filtered((prop));

    int base = 1;
    int finalVal = mods.apply(base);

    ModifiedPropertyValue ret(PropertyValue(prop, base), finalVal, mods);
    return ret;
}

bool GameContext::checkCharacterIgnoreMonsterAttribute(const Character *c, AH::Common::MonsterData::MonsterAttribute attr)
{
    int ignored = getCharacterIgnoredMonsterAttributes(c);

    return (ignored != 0 && (ignored & attr) == attr);
}

AH::Common::MonsterData::MonsterAttributes GameContext::getCurCharacterIgnoredMonsterAttributes()
{
    return getCharacterIgnoredMonsterAttributes(m_player->getCharacter());
}

AH::Common::MonsterData::MonsterAttributes GameContext::getCharacterIgnoredMonsterAttributes(const Character *c)
{
    PropertyModificationList mods = c->getPropertyModifiers().filtered(PropertyValue::Ignore_MonsterAttributes);
    // TODO: Can other modifiers apply?
    return static_cast<AH::Common::MonsterData::MonsterAttributes> (mods.apply(0));
}

ModifiedPropertyValue GameContext::getCurMonsterProperty(AH::Common::PropertyValueData::Property property)
{
    return getMonsterProperty(m_monster, property);
}

ModifiedPropertyValue GameContext::getMonsterProperty(const Monster *m, AH::Common::PropertyValueData::Property property)
{
    Q_ASSERT(PropertyValue::isMonsterProperty(property));

    int base = 0;
    switch (property)
    {
    case PropertyValue::Monster_CombatDamage:
        base = m->baseCombatDamage();
        break;
    case PropertyValue::Monster_CombatAdjustment:
        base = m->baseCombatAdjustment();
        break;
    case PropertyValue::Monster_HorrorDamage:
        base = m->baseHorrorDamage();
        break;
    case PropertyValue::Monster_HorrorAdjustment:
        base = m->baseHorrorAdjustment();
        break;
    case PropertyValue::Monster_Awareness:
        base = m->baseAwareness();
        break;
    case PropertyValue::Monster_Toughness:
        base = m->baseToughness();
        break;
    case PropertyValue::Monster_Movement:
        base = m->baseMovement();
        break;
    default:
        Q_ASSERT_X(false, "GameContext::getMonsterProperty", "Property not defined");
    }

    PropertyModificationList mods = m->getModifications().filtered(property);
    mods += m_game->getGameModifiers().filtered(property);
    // ANCIENT ONE!
    mods += ancientOne()->getMonsterModifications(m->typeId()).filtered(property);

    int finalVal = mods.apply(base);

    return ModifiedPropertyValue(PropertyValue(property, base), finalVal, mods);
}

ModifiedPropertyValue GameContext::getGameProperty(PropertyValue::Property property)
{
    Q_ASSERT(PropertyValue::isGameProperty(property));

    int playerCount = gGame->getPlayers().count();
    int base = 0;
    switch (property)
    {
    case PropertyValue::Game_SealClueCost:
        base = gGame->getGateSealClueCost();
        break;
    case PropertyValue::Game_AllowMaskMonster:
        base = 0;
        break;
    case PropertyValue::Game_MaxGateCount:
        base = 8-(playerCount-1)/2;
        break;
    case PropertyValue::Game_MaxBoardMonsterCount:
        if (getGameProperty(PropertyValue::Game_TerrorLevel).finalVal() >= getGameProperty(PropertyValue::Game_OverrunArkhamTerrorLevel).finalVal()) {
            base = 10000;
        } else {
            base = playerCount+3;
        }
        break;
    case PropertyValue::Game_MaxOutskirtsMonsterCount:
        base = 8-playerCount;
        break;
    case PropertyValue::Game_TerrorLevel:
        base = gGame->terrorLevel();
        break;
    case PropertyValue::Game_MonsterCountFromGates:
        if (playerCount >= 5) {
            base = 2;
        } else {
            base = 1;
        }
        break;
    case PropertyValue::Game_CloseGeneralStoreTerrorLevel:
        base = 3;
        break;
    case PropertyValue::Game_CloseCuriosityShoppeTerrorLevel:
        base = 6;
        break;
    case PropertyValue::Game_CloseYeOldeMagickShoppeTerrorLevel:
        base = 9;
        break;
    case PropertyValue::Game_OverrunArkhamTerrorLevel:
        base = 10;
        break;
    case PropertyValue::Game_SealedGatesToWin:
        base = 6;
        break;
    case PropertyValue::Game_GateTrophiesToWin:
        base = playerCount;
        break;

    default:
        Q_ASSERT_X(false, "GameContext::getCharacterProperty", "Property not defined");
    }

    PropertyModificationList mods = gGame->getGameModifiers().filtered(property);

    int finalVal = mods.apply(base);

    ModifiedPropertyValue ret(PropertyValue(property, base), finalVal, mods);
    return ret;
}
