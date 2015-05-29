#include "character.h"
#include <cost.h>
#include "game/game.h"
#include "game/gameaction.h"
#include "game/gameoption.h"
#include "game/gameobject.h"
#include "monster.h"
#include "gate.h"
#include "game/choicehelper.h"
#include <QDebug>

using namespace AH::Common;

Character::Character(Investigator *i)
:   m_investigator(i),
    m_dirty(false),
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
    m_curDmgStamina(0),
    m_curDmgSanity(0),
    m_explorededGate(NULL)
{
    instantiateFromInvestigator();
}

CharacterData *Character::data()
{
    // Synchonize data with character
    m_attrSettings.clear();
    foreach (AttributeSlider s, m_sliders) {
        m_attrSettings << s.currentSettingPos();
    }

    m_inventoryIds.clear();
    foreach (const GameObject *obj, m_inventory) {
        m_inventoryIds << obj->id();
    }

    m_monsterMarkerIds.clear();
    m_monsterToughness = 0;
    foreach (const Monster *m, m_monsterMarkers) {
        m_monsterMarkerIds << m->id();
        m_monsterToughness += m->toughness();
    }

    m_gateMarkerIds.clear();
    foreach (const Gate *g, m_gateMarkers) {
        m_gateMarkerIds << g->id();
    }

    m_fieldId = m_field->id();

    return CharacterData::data();
}

PropertyModificationList Character::getPropertyModifiers() const
{
    PropertyModificationList ret;
    foreach (GameObject *obj, m_inventory) {
        ret.append(obj->getModifications());
    }
    return ret;
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

void Character::addToInventory(GameObject *obj)
{
    if (!obj) return;
    if (obj->owner() == NULL) {
        if (!obj->onAddToInventory(this)) {
            // Don't add
            return;
        }
        obj->setOwner(this);
        m_inventory << obj;
        gGame->characterDirty(this);
    } else if (obj->owner() != this) {
        Q_ASSERT(false);
    }
}

void Character::removeFromInventory(GameObject *obj)
{
    if (!obj) return;
    if (obj->owner() == this) {
        if (!obj->onRemoveFromInventory(this)) {
            // don't remove
            return;
        }
        m_inventory.removeAll(obj);
        obj->setOwner(NULL);
        gGame->characterDirty(this);
    } else if (obj->owner() != NULL) {
        Q_ASSERT(false);
    }
}

QList<AttributeSlider> Character::getFocusAttributes() const
{
    return m_sliders;
}

QList<AttributeSlider> &Character::getModifyableFocusAttributes()
{
    return m_sliders;
}

bool Character::canPay(const Cost &cost) const
{
    if (cost.getAlternatives().isEmpty()) {
        return true;
    }
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
        case CostItem::Pay_MonsterToughness:
        {
            int sum = 0;
            foreach (Monster *m, m_monsterMarkers) {
                sum += m->toughness();
            }
            if (sum < i.amount)
                return false;
            break;
        }
        default:
            Q_ASSERT_X(false, "CanPay", "Not yet implemented");
        }
    }
    return true;
}

bool Character::pay(const Cost &cost)
{
    /*
    if (cost.getAlternatives().isEmpty()) {
        return true;
    }
    if (cost.getAlternatives().size() == 1) {
        return pay(cost.getAlternatives()[0]);
    }

    // Let user decide:
    // TODO
    return pay(cost.getAlternatives()[0]);
    */
    AH::Common::CostList sel;
    bool ok = ChoiceHelper::choosePayment(this, cost, sel);
    if (ok) {
        return pay(sel);
    }
    return false;
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
            for (int j = 0; j < i.amount; ++j) {
                // TODO: Return to stash?
                m_gateMarkers.removeFirst();
            }
            break;
        case CostItem::Pay_MonsterTrophy:
            // TODO: Let user choose?
            for (int j = 0; j < i.amount; ++j) {
                gGame->returnMonster(m_monsterMarkers.first());
                m_monsterMarkers.removeFirst();
            }
            break;
        case CostItem::Pay_MonsterToughness:
        {
            // TODO: Better algorithm
            // TODO: Let user choose?
            int sum = 0;
            for (int j = 0; sum < i.amount && m_monsterMarkers.size() > 0; ++j) {
                Monster *m = m_monsterMarkers.first();
                sum += m->toughness();
                gGame->returnMonster(m);
                m_monsterMarkers.removeFirst();
            }
            break;
        }

            //if (m_money < i.amount) return false; continue;
        default:
            Q_ASSERT_X(false, "CanPay", "Not yet implemented");
        }
    }

    bool res = commitDamage();

    gGame->characterDirty(this);

    return res;
}

void Character::loseClues()
{
    m_clues = (m_clues+1)/2;
    gGame->characterDirty(this);
}

void Character::loseHalfPossessions()
{
    ChoiceHelper::loseHalfPossesions(this);
}

void Character::losePossessions(int count)
{
    ChoiceHelper::losePossessions(this, count);
}

void Character::arrest()
{
    loseClues();
    gGame->board()->field(AH::Common::FieldData::ET_PoliceStation)->placeCharacter(this);
    setDelayed(true);
    setSetout(true);
    // Should actually set out next round...
}

void Character::unconscious()
{
    if (field()->type() == AH::Common::FieldData::OtherWorld) {
        lostInSpaceAndTime();
        return;
    }

    m_curStamina = 1;
    loseHalfPossessions();
    gGame->board()->field(AH::Common::FieldData::UT_StMarysHospital)->placeCharacter(this);
    setSetout(true);
    gGame->characterDirty(this);
    //gGame->boardDirty();
}

void Character::insane()
{
    if (field()->type() == AH::Common::FieldData::OtherWorld) {
        lostInSpaceAndTime();
        return;
    }

    m_curSanity = 1;
    loseHalfPossessions();
    gGame->board()->field(AH::Common::FieldData::DT_ArkhamAsylum)->placeCharacter(this);
    setSetout(true);
    gGame->characterDirty(this);
    //gGame->boardDirty();
}

void Character::devour()
{
    // Check for anciant one fight
    if (gGame->context().phase() == AH::EndFightPhase) {
        // KILL HIM!
        gGame->killPlayer(gGame->context().player());
        return;
    }

    // Simply replace investigator
    gGame->returnInvestigator(investigator());
    Investigator *newInv = gGame->drawInvestigator();
    gGame->replacePlayerCharacter(gGame->playerForCharacter(this), newInv);
}

void Character::lostInSpaceAndTime()
{
    m_curSanity = qMax(1, m_curSanity);
    m_curStamina = qMax(1, m_curStamina);
    loseHalfPossessions();
    setDelayed(true);
    gGame->board()->field(AH::Common::FieldData::Sp_SpaceAndTime)->placeCharacter(this);
    gGame->characterDirty(this);
    //gGame->boardDirty();
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

    // Unique ability
    GameObject *ua = gGame->registry()->findObjectById(m_investigator->uniqueAbilityId());
    if (ua) {
        addToInventory(ua);
    } else {
        qWarning() << "Unique ability" << m_investigator->uniqueAbilityId() << "for investigator" << m_investigator->id() << "is not an object";
    }

    // Game will give Random and fixed possesion
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
    m_curDmgStamina += amount;

}

void Character::damageSanity(int amount)
{
    m_curDmgSanity += amount;
}

bool Character::commitDamage()
{
    if (m_curDmgSanity == 0 && m_curDmgStamina == 0) {
        return true;
    }
    m_curStamina = qMax(0, m_curStamina - m_curDmgStamina);
    m_curSanity = qMax(0, m_curSanity - m_curDmgSanity);

    m_curDmgSanity = m_curDmgStamina = 0;

    if (m_curSanity == 0 && m_curStamina == 0) {
        devour();
        return false;
    }

    if (m_curSanity == 0) {
        insane();
        return false;
    }
    if (m_curStamina == 0) {
        unconscious();
        return false;
    }

    gGame->characterDirty(this);
    return true;
}

void Character::addStamina(int amount)
{
    if (m_curDmgStamina > 0) {
        int newAmount = amount - m_curDmgStamina;
        m_curDmgStamina = qMax(0, m_curDmgStamina - amount);
        amount = qMax(0, newAmount);
    }
    int maxStamina = gGame->context().getCharacterProperty(this, PropertyValue::Prop_MaxStamina).finalVal();
    int newStamina = qMin(maxStamina, m_curStamina + amount);
    if (newStamina == m_curStamina)
        return;

    m_curStamina = newStamina;
    gGame->characterDirty(this);
}

void Character::addSanity(int amount)
{
    if (m_curDmgSanity > 0) {
        int newAmount = amount - m_curDmgSanity;
        m_curDmgSanity = qMax(0, m_curDmgSanity - amount);
        amount = qMax(0, newAmount);
    }
    int maxSanity = gGame->context().getCharacterProperty(this, PropertyValue::Prop_MaxSanity).finalVal();
    int newSanity  = qMin(maxSanity, m_curSanity + amount);
    if (newSanity == m_curSanity)
        return;

    m_curSanity = newSanity;
    gGame->characterDirty(this);
}

void Character::restoreStamina()
{
    m_curDmgStamina = 0;
    int maxStamina = gGame->context().getCharacterProperty(this, PropertyValue::Prop_MaxStamina).finalVal();

    if (m_curStamina == maxStamina)
        return;

    m_curStamina = maxStamina;
    gGame->characterDirty(this);
}

void Character::restoreSanity()
{
    m_curDmgSanity = 0;
    int maxSanity = gGame->context().getCharacterProperty(this, PropertyValue::Prop_MaxSanity).finalVal();

    if (m_curSanity == maxSanity)
        return;

    m_curSanity = maxSanity;
    gGame->characterDirty(this);
}

void Character::preventDamageStamina(int amount)
{
    // TODO: Is this right?
    m_curDmgStamina = qMax(m_curDmgStamina - amount, 0);
}

void Character::preventDamageSanity(int amount)
{
    // TODO: Is this right?
    m_curDmgSanity = qMax(m_curDmgSanity - amount, 0);
}

bool Character::returnToArkham()
{
    if (!field()->backGates().isEmpty()) {
        // TODO: Let choose gate
        Gate *p = field()->backGates().at(0);
        p->comeBack(this);
        //gGame->notifier()->actionExecute(this, QString("Returned to %1").arg(p->field()->name()));
        return true;
    }
    return false;
}

