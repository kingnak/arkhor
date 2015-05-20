#include "gameobject.h"
#include "gameaction.h"
#include "gameoption.h"
#include "game.h"
#include "game/player.h"
#include "character.h"
#include "cost.h"
#include "dietesthelper.h"
#include <QStringList>

QStringList GameObject::actionIds() const
{
    QStringList ret;
    foreach (const GameAction *a, getActions()) {
        ret << a->id();
    }
    return ret;
}

QStringList GameObject::optionIds() const
{
    QStringList ret;
    foreach (const GameOption *o, getOptions()) {
        ret << o->id();
    }
    return ret;
}

bool GameObject::isFightObject()
{
    foreach (PropertyModification m, getModifications()) {
        switch (m.affectedProperty()) {
        case PropertyValue::Damage_General:
        case PropertyValue::Damage_Physical:
        case PropertyValue::Damage_Magical:
            return true;
        }
    }
    return false;
}

bool GameObject::equip(Player *p)
{
    if (!isFightObject()) {
        return false;
    }

    if (m_isEquipped) {
        return true;
    }

    if (m_bIsExhausted) {
        return false;
    }

    // Special case: Spells => Cast
    if (m_type == AH::Obj_Spell) {
        if (!cast(p)) {
            return false;
        }
    }

    m_isEquipped = true;
    return true;
}

bool GameObject::unequip()
{
    if (!isFightObject()) {
        return false;
    }

    if (!m_isEquipped) {
        return true;
    }

    // Special case: Spells => Exhaust
    if (m_type == AH::Obj_Spell) {
        exhaust();
    }

    m_isEquipped = false;
    return true;
}

bool GameObject::cast(Player *p)
{
    if (m_type != AH::Obj_Spell) {
        return false;
    }

    if (m_bIsExhausted) {
        return false;
    }

    // Pay cost
    ModifiedPropertyValue cc = gGame->context().getCharacterProperty(p->getCharacter(), PropertyValue::Prop_CastCost);
    if (cc.finalVal() != 0) {
        AH::Common::CostList cost;
        cost << AH::Common::CostItem(AH::Common::CostItem::Pay_Sanity, cc.finalVal());
        if (!p->getCharacter()->pay(cost)) {
            return false;
        }
    }

    // Is exhausted now
    exhaust();

    // Test will
    if (this->castTarget() > 0) {
        ModifiedPropertyValue ca = gGame->context().getCharacterProperty(p->getCharacter(), PropertyValue::Prop_CastAdjustment);
        DieTestHelper::DieTestSpec test = DieTestHelper::createSkillTest(p->getCharacter(), AH::Skill_Will, ca.finalVal(), this->castTarget());
        DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(p, test);

        if (!res.boolResult) {
            return false;
        }
    }

    return true;
}
