#include "character.h"
#include <cost.h>
#include "game/game.h"
#include "game/gameaction.h"
#include "game/gameoption.h"
#include "game/gameobject.h"

using namespace AH::Common;

Character::Character(Investigator *i)
:   m_investigator(i),
    m_field(NULL),
    m_maxFocus(0),
    /*
    m_maxStamina(0),
    m_curStamina(0),
    m_maxSanity(0),
    m_curSanity(0),
    m_curFocus(0),
    m_curMovement(0),
    m_money(0),
    m_isDelayed(false),
    m_owPhase(AH::NoPhase)
    */
    m_explorededGate(NULL)
{
    instantiateFromInvestigator();
}

QList<GameAction *> Character::getActions(AH::GamePhase phase)
{
    QList<GameAction *> acts;
    foreach (GameObject *obj, m_inventory) {
        foreach (GameAction *a, obj->getActions()) {
            if (a->phases().testFlag(phase)) {
                acts << a;
            }
        }
    }
    return acts;
}

QList<GameOption *> Character::getOptions(AH::GamePhase phase)
{
    QList<GameOption *> opts;
    foreach (GameObject *obj, m_inventory) {
        foreach (GameOption *o, obj->getOptions()) {
            if (o->phases().testFlag(phase)) {
                opts << o;
            }
        }
    }
    return opts;
}

QList<GameObject *> &Character::inventory()
{
    return m_inventory;
}

QList<AttributeSlider> Character::getFocusAttributes() const
{
    return m_sliders;
}

QList<AttributeSlider> &Character::getModifyableFocusAttributes()
{
    return m_sliders;
}

const Gate *Character::getExploredGate() const
{
    return m_explorededGate;
}

bool Character::canPay(const Cost &cost) const
{
    foreach (CostList cl, cost.getAlternatives()) {
        if (canPay(cl)) return true;
    }
    return false;
}

bool Character::canPay(const CostList &cost) const
{
    foreach (CostItem i, cost) {
        switch (i.type) {
        case CostItem::Pay_None:
            break;
        case CostItem::Pay_Money:
            if (m_money < i.amount)
                return false;
            break;
        case CostItem::Pay_Clue:
            if (m_clues < i.amount)
                return false;
            break;
        case CostItem::Pay_Stamina:
            if (m_curStamina < i.amount)
                return false;
            break;
        case CostItem::Pay_Sanity:
            if (m_curSanity < i.amount)
                return false;
            break;
        case CostItem::Pay_Movement:
            if (gGame->context().getCharacterProperty(this, PropertyValue::Prop_Movement).finalVal() < i.amount)
                return false;
            break;
        case CostItem::Pay_GateTrophy:
            if (m_gateMarkers.size() < i.amount)
                return false;
            break;
        case CostItem::Pay_MonsterTrophy:
            if (m_monsterMarkers.size() < i.amount)
                return false;
            break;
        case CostItem::Pay_MonsterStamina:
            //if (m_money < i.amount) return false; continue;
            Q_ASSERT_X(false, "CanPay", "Not yet implemented");
        }
    }
    return true;
}

bool Character::pay(const CostList &cost)
{
    if (!canPay(cost)) return false;

    foreach (CostItem i, cost) {
        switch (i.type) {
        case CostItem::Pay_None:
            break;
        case CostItem::Pay_Money:
            m_money -= i.amount;
            break;
        case CostItem::Pay_Clue:
            m_clues -= i.amount;
            break;
        case CostItem::Pay_Stamina:
            damageStamina(i.amount);
            break;
        case CostItem::Pay_Sanity:
            damageSanity(i.amount);
            break;
        case CostItem::Pay_Movement:
            m_movementPoints -=  i.amount;
            break;
        case CostItem::Pay_GateTrophy:
            // TODO: Return to stash
            for (int j = 0; j < i.amount; ++j)
                m_gateMarkers.removeFirst();
            break;
        case CostItem::Pay_MonsterTrophy:
            // TODO: Return to stash
            // TODO: Let user choose
            for (int j = 0; j < i.amount; ++j)
                m_monsterMarkers.removeFirst();
            break;
        case CostItem::Pay_MonsterStamina:
            //if (m_money < i.amount) return false; continue;
            Q_ASSERT_X(false, "CanPay", "Not yet implemented");
        }
    }

    // TODO: Verify Sanity and Stamina

    return true;
}

void Character::unconscious()
{
    // TODO
}

void Character::insane()
{
    // TODO
}

void Character::devour()
{
    // TODO
}

void Character::instantiateFromInvestigator()
{
    m_investigatorId = m_investigator->id();
    m_id = "CH_"+m_investigatorId;
    m_curStamina = m_maxStamina = m_investigator->stamina();
    m_curSanity = m_maxSanity = m_investigator->sanity();
    m_money = m_investigator->money();
    m_clues = m_investigator->clues();
    m_curFocus = m_maxFocus = m_investigator->focus();

    m_sliders.clear();
    AttributeSlider ss;
    foreach (AH::Common::InvestigatorData::AttributeValuePair p, m_investigator->attrSpeedSneak()) {
        ss.addAttributePair(AttributePair(
            AttributeValue(AH::Attr_Speed, p.first),
            AttributeValue(AH::Attr_Sneak, p.second)
            ));
    }
    m_sliders.append(ss);

    AttributeSlider fw;
    foreach (AH::Common::InvestigatorData::AttributeValuePair p, m_investigator->attrFightWill()) {
        fw.addAttributePair(AttributePair(
            AttributeValue(AH::Attr_Fight, p.first),
            AttributeValue(AH::Attr_Will, p.second)
            ));
    }
    m_sliders.append(fw);

    AttributeSlider ll;
    foreach (AH::Common::InvestigatorData::AttributeValuePair p, m_investigator->attrLoreLuck()) {
        ll.addAttributePair(AttributePair(
            AttributeValue(AH::Attr_Lore, p.first),
            AttributeValue(AH::Attr_Luck, p.second)
            ));
    }
    m_sliders.append(ll);

    m_fieldId = m_investigator->startFieldId();

    // TODO Unique ability

    // TODO LATER:
    // Random and fixed possesion
}

int Character::getAttributeValue(AH::Attribute attr) const
{
    foreach (AttributeSlider s, m_sliders) {
        AttributePair p = s.currentSetting();
        if (p.first().attribute == attr)
            return p.first().value;
        if (p.second().attribute == attr)
            return p.second().value;
    }

    return 0;
}

void Character::damageStamina(int amount)
{
    m_curStamina = qMax(0, m_curStamina - amount);
    if (m_curStamina == 0) {
        unconscious();
    }
}

void Character::damageSanity(int amount)
{
    m_curSanity = qMax(0, m_curSanity - amount);
    if (m_curSanity == 0)
        insane();
}

void Character::addStamina(int amount)
{
    int maxStamina = gGame->context().getCharacterProperty(this, PropertyValue::Prop_MaxStamina).finalVal();
    m_curStamina = qMin(maxStamina, m_curStamina + amount);
}

