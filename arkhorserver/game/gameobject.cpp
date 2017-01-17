#include "gameobject.h"
#include "gameaction.h"
#include "gameoption.h"
#include "game.h"
#include "game/player.h"
#include "character.h"
#include "cost.h"
#include "dietesthelper.h"
#include <QStringList>

void GameObject::exhaust()
{
    if (m_bExhaustable) {
        if (!m_bIsExhausted) {
            m_bIsExhausted = true;
            gGame->invalidateObject(this->id());
        }
    }
}

void GameObject::refresh()
{
    if (m_bExhaustable) {
        if (m_bIsExhausted) {
            m_bIsExhausted = false;
            gGame->invalidateObject(this->id());
        }
    }
}

bool GameObject::onAddToInventory(Character *c)
{
    if (type() == AH::Obj_Blessing_Curse) {
        // Can only have one Blessing XOR Curse
        // If adding one, and there is already the same,
        // do nothing. If it is other type, remove other.
        // Else, simply add
        foreach (GameObject *o, c->inventory()) {
            if (o->type() == AH::Obj_Blessing_Curse) {
                if (o->typeId() == typeId()) {
                    // same, don't add.
                    return false;
                } else {
                    // Other type. Remove it and don't add
                    o->returnToDeck();
                    return false;
                }
            }
        }
        // Had none, do default
    }
    return true;
}

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
        default:
            break;
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

void GameObject::returnToDeck()
{
    gGame->returnObject(this);
}

QList<AH::Common::PropertyModificationData> GameObject::getModificationData() const
{
    QList<AH::Common::PropertyModificationData> ret;
    foreach (PropertyModification m, getModifications()) {
        ret << *(m.data());
    }
    return ret;
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
    PropertyModificationList cc = gGame->context().player()->getCharacter()->getPropertyModifiers().filtered(PropertyValue::Prop_CastCost);
    cc += gGame->getGameModifiers().filtered(PropertyValue::Prop_CastCost);
    PropertyValue basecc(PropertyValue::Prop_CastCost, this->castCost());
    ModifiedPropertyValue modcc(basecc, cc.apply(basecc.value()), cc);
    if (modcc.finalVal() != 0) {
        AH::Common::CostList cost;
        cost << AH::Common::CostItem(AH::Common::CostItem::Pay_Sanity, modcc.finalVal());
        if (!p->getCharacter()->pay(cost)) {
            return false;
        }
    }

    // Is exhausted now
    exhaust();

    // Test will
    if (this->castTarget() > 0) {
        PropertyModificationList ca = gGame->context().player()->getCharacter()->getPropertyModifiers().filtered(PropertyValue::Prop_CastAdjustment);
        ca += gGame->getGameModifiers().filtered(PropertyValue::Prop_CastAdjustment);
        PropertyValue baseca(PropertyValue::Prop_CastAdjustment, this->castAdjustment());
        ModifiedPropertyValue modca(basecc, ca.apply(baseca.value()), ca);

        DieTestHelper::DieTestSpec test = DieTestHelper::createSkillTest("Cast " + name(), p->getCharacter(), AH::Skill_Will, modca.finalVal(), this->castTarget());
        DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(p, test);

        if (!res.boolResult) {
            return false;
        }
    }

    return true;
}
