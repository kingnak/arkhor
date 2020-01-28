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

int Character::maxStamina() const
{
    return gGame->context().getCharacterProperty(this, PropertyValue::Prop_MaxStamina).finalVal();
}

int Character::maxSanity() const
{
    return gGame->context().getCharacterProperty(this, PropertyValue::Prop_MaxSanity).finalVal();
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
        gGame->notifier()->notifySimple("{C} drew {D}", gGame->playerForCharacter(this), obj->name());
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
        gGame->notifier()->notifySimple("{C} lost {D}", gGame->playerForCharacter(this), obj->name());
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

    QStringList pays;
    foreach (CostItem i, cost) {
        switch (i.type) {
        case CostItem::Pay_None:
            break;
        case CostItem::Pay_Money:
            m_money -= i.amount;
            pays << QString("%1$").arg(i.amount);
            break;
        case CostItem::Pay_Clue:
            m_clues -= i.amount;
            pays << QString("%1 clues").arg(i.amount);
            break;
        case CostItem::Pay_Stamina:
            damageStamina(i.amount);
            pays << QString("%1 stamina").arg(i.amount);
            break;
        case CostItem::Pay_Sanity:
            damageSanity(i.amount);
            pays << QString("%1 sanity").arg(i.amount);
            break;
        case CostItem::Pay_Movement:
            m_movementPoints -=  i.amount;
            pays << QString("%1 movement points").arg(i.amount);
            break;
        case CostItem::Pay_GateTrophy:
            for (int j = 0; j < i.amount; ++j) {
                // TODO: Return to stash?
                m_gateMarkers.removeFirst();
            }
            pays << QString("%1 gate trophies").arg(i.amount);
            break;
        case CostItem::Pay_MonsterTrophy:
            // TODO: Let user choose?
            for (int j = 0; j < i.amount; ++j) {
                gGame->returnMonster(m_monsterMarkers.first());
                m_monsterMarkers.removeFirst();
            }
            pays << QString("%1 monster trophies").arg(i.amount);
            break;
        case CostItem::Pay_MonsterToughness:
        {
            // TODO: Better algorithm
            // TODO: Let user choose?
            int sum = 0;
            int ct = 0;
            for (int j = 0; sum < i.amount && m_monsterMarkers.size() > 0; ++j) {
                Monster *m = m_monsterMarkers.first();
                sum += m->toughness();
                gGame->returnMonster(m);
                m_monsterMarkers.removeFirst();
                ct++;
            }
            pays << QString("%2 monster trophies for %1 toughness").arg(i.amount).arg(ct);
            break;
        }

            //if (m_money < i.amount) return false; continue;
        default:
            Q_ASSERT_X(false, "CanPay", "Not yet implemented");
        }
    }

    bool res = commitDamage();

    gGame->characterDirty(this);
    if (!pays.isEmpty())
    {
        gGame->notifier()->notifySimple("{C} payed {D}", gGame->playerForCharacter(this), pays.join(", "));
    }

    return res;
}

void Character::loseClues()
{
    int cluesOld = m_clues;
    m_clues = (m_clues+1)/2;
    gGame->characterDirty(this);
    gGame->notifier()->notifySimple("{C} lost {D}", gGame->playerForCharacter(this), QString("%1 clues").arg(cluesOld - m_clues));
}

void Character::loseHalfPossessions(const QString &sourceId)
{
    ChoiceHelper::loseHalfPossesions(this, sourceId);
}

void Character::losePossessions(int count, const QString &sourceId)
{
    ChoiceHelper::losePossessions(this, count, sourceId);
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
    // Check for ancient one fight
    if (gGame->context().phase() == AH::EndFightPhase) {
        devour();
        return;
    }

    if (!gGame->handleUnconscious(this)) {
        return;
    }

    if (field()->type() == AH::Common::FieldData::OtherWorld) {
        lostInSpaceAndTime();
        return;
    }

    gGame->notifier()->notifySpecific("You are unconscious!", "{C} is unconscious", gGame->playerForCharacter(this));
    m_curStamina = qMin(1, maxStamina());
    if (m_curStamina == 0) {
        devour();
        return;
    }
    loseHalfPossessions();
    gGame->board()->field(AH::Common::FieldData::UT_StMarysHospital)->placeCharacter(this);
    setSetout(true);
    gGame->characterDirty(this);
    //gGame->boardDirty();
}

void Character::insane()
{
    // Check for ancient one fight
    if (gGame->context().phase() == AH::EndFightPhase) {
        devour();
        return;
    }

    if (!gGame->handleInsane(this)) {
        return;
    }

    if (field()->type() == AH::Common::FieldData::OtherWorld) {
        lostInSpaceAndTime();
        return;
    }

    gGame->notifier()->notifySpecific("You are insane!", "{C} is insane", gGame->playerForCharacter(this));
    m_curSanity = qMin(1, maxSanity());
    if (m_curSanity == 0) {
        devour();
        return;
    }
    loseHalfPossessions();
    gGame->board()->field(AH::Common::FieldData::DT_ArkhamAsylum)->placeCharacter(this);
    setSetout(true);
    gGame->characterDirty(this);
    //gGame->boardDirty();
}

void Character::devour()
{
    // Check for ancient one fight
    if (gGame->context().phase() == AH::EndFightPhase) {
        // KILL HIM!
        gGame->characterDirty(this);
        gGame->killPlayer(gGame->playerForCharacter(this));
        return;
    }

    gGame->notifier()->notifySpecific("You were devoured!", "{C} was devoured", gGame->playerForCharacter(this));
    // Simply replace investigator
    gGame->returnInvestigator(investigator());
    Investigator *newInv = gGame->drawInvestigator();
    gGame->replacePlayerCharacter(gGame->playerForCharacter(this), newInv);
}

void Character::lostInSpaceAndTime()
{
    if (!gGame->handleLostInSpaceAndTime(this)) {
        return;
    }

    m_curSanity = qMax(1, m_curSanity);
    m_curStamina = qMax(1, m_curStamina);
    gGame->notifier()->notifySpecific("You are lost in time and space!", "{C} is lost in time and space", gGame->playerForCharacter(this));
    loseHalfPossessions();
    setDelayed(true);
    gGame->board()->field(AH::Common::FieldData::Sp_SpaceAndTime)->placeCharacter(this);
    gGame->characterDirty(this);
    //gGame->boardDirty();
}

void Character::instantiateFromInvestigator()
{
    m_investigatorId = m_investigator->id();
    m_investigatorData = *m_investigator->data();
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
    // Prevent race condition when client asks for investigator, before it is placed
    m_field = gGame->board()->field(m_fieldId);

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

void Character::addClue(int amount)
{
    m_clues += amount;
    gGame->characterDirty(this);
    gGame->notifier()->notifySimple("{C} gained {D}", gGame->playerForCharacter(this), QString("%1 clues").arg(amount));
}

void Character::addMoney(int amount)
{
    m_money += amount;
    gGame->characterDirty(this);
    gGame->notifier()->notifySimple("{C} gained {D}", gGame->playerForCharacter(this),QString("%1$").arg(amount));
}

void Character::loseMoney(int amount)
{
    int moneyOld = m_money;
    m_money = qMax(m_money-amount,0);
    gGame->characterDirty(this);
    gGame->notifier()->notifySimple("{C} lost {D}", gGame->playerForCharacter(this),QString("%1$").arg(moneyOld-m_money));
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
    bool maxIsZero = false;
    if (m_curSanity > maxSanity()) {
        m_curSanity = maxSanity();
        setDirty();
        if (m_curSanity <= 0) maxIsZero = true;
    }
    if (m_curStamina > maxStamina()) {
        m_curStamina = maxStamina();
        setDirty();
        if (m_curStamina <= 0) maxIsZero = true;
    }

    if (maxIsZero) {
        m_curDmgSanity = m_curDmgStamina = 0;
        devour();
        return false;
    }

    if (m_curDmgSanity <= 0 && m_curDmgStamina <= 0) {
        return true;
    }

    int preventSta, preventSan;
    gGame->preventDamageHelper(gGame->playerForCharacter(this), m_curDmgStamina, m_curDmgSanity, preventSta, preventSan);

    if (m_curDmgSanity <= 0 && m_curDmgStamina <= 0) {
        return true;
    }

    m_curStamina = qMax(0, m_curStamina - m_curDmgStamina);
    m_curSanity = qMax(0, m_curSanity - m_curDmgSanity);

    QStringList notify;
    if (m_curDmgStamina > 0) {
        notify << QString("%1 stamina").arg(m_curDmgStamina);
    }
    if (m_curDmgSanity > 0) {
        notify << QString("%1 sanity").arg(m_curDmgSanity);
    }
    gGame->notifier()->notifySimple("{C} was damaged {D}", gGame->playerForCharacter(this), notify.join(" and "));

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
    /*
    if (m_curDmgStamina > 0) {
        int newAmount = amount - m_curDmgStamina;
        m_curDmgStamina = qMax(0, m_curDmgStamina - amount);
        if (newAmount - amount > 0) gGame->notifier()->notifySimple("{C} prevented {D} stamina damage", QString::number(newAmount - amount));
        amount = qMax(0, newAmount);
    }
    */
    int maxStamina = gGame->context().getCharacterProperty(this, PropertyValue::Prop_MaxStamina).finalVal();
    int newStamina = qMin(maxStamina, m_curStamina + amount);
    if (newStamina == m_curStamina)
        return;

    m_curStamina = newStamina;
    gGame->characterDirty(this);
    if (amount > 0) gGame->notifier()->notifySimple("{C} healed {D} stamina", gGame->playerForCharacter(this), QString::number(amount));
}

void Character::addSanity(int amount)
{
    /*
    if (m_curDmgSanity > 0) {
        int newAmount = amount - m_curDmgSanity;
        m_curDmgSanity = qMax(0, m_curDmgSanity - amount);
        if (newAmount - amount > 0) gGame->notifier()->notifySimple("{C} prevented {D} sanity damage", QString::number(newAmount - amount));
        amount = qMax(0, newAmount);
    }
    */
    int maxSanity = gGame->context().getCharacterProperty(this, PropertyValue::Prop_MaxSanity).finalVal();
    int newSanity  = qMin(maxSanity, m_curSanity + amount);
    if (newSanity == m_curSanity)
        return;

    m_curSanity = newSanity;
    gGame->characterDirty(this);
    if (amount > 0) gGame->notifier()->notifySimple("{C} healed {D} sanity", gGame->playerForCharacter(this), QString::number(amount));
}

void Character::restoreStamina()
{
    m_curDmgStamina = 0;
    int maxStamina = gGame->context().getCharacterProperty(this, PropertyValue::Prop_MaxStamina).finalVal();

    if (m_curStamina == maxStamina)
        return;

    m_curStamina = maxStamina;
    gGame->characterDirty(this);
    gGame->notifier()->notifySimple("{C} restored stamina", gGame->playerForCharacter(this));
}

void Character::restoreSanity()
{
    m_curDmgSanity = 0;
    int maxSanity = gGame->context().getCharacterProperty(this, PropertyValue::Prop_MaxSanity).finalVal();

    if (m_curSanity == maxSanity)
        return;

    m_curSanity = maxSanity;
    gGame->characterDirty(this);
    gGame->notifier()->notifySimple("{C} restored sanity", gGame->playerForCharacter(this));
}

int Character::freeHands() const
{
    int hands = gGame->context().getCharacterProperty(this, PropertyValue::Prop_HandCount).finalVal();
    for (auto o : m_inventory) {
        if (o->isEquipped()) {
            hands -= o->handCount();
        }
    }
    return hands;
}

/*
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
*/

void Character::addMonsterTrophy(Monster *m)
{
    if (!m) return;
    m_monsterMarkers.append(m);
    gGame->characterDirty(this);
    gGame->notifier()->notifySimple("{C} received a monster tropy", gGame->playerForCharacter(this));
}

void Character::addGateTrophy(Gate *p)
{
    if (!p) return;
    m_gateMarkers.append(p);
    gGame->characterDirty(this);
    gGame->notifier()->notifySimple("{C} received a gate tropy", gGame->playerForCharacter(this));
}

void Character::setExploredGate(const Gate *p)
{
    m_explorededGate = p;
    gGame->characterDirty(this);
}

void Character::setDelayed(bool delay)
{
    if (m_delayed == delay) return;
    m_delayed = delay;
    gGame->characterDirty(this);
    gGame->notifier()->notifySimple("{C} is {D}delayed", gGame->playerForCharacter(this), delay ? "":"no longer ");
}

void Character::setSetout(bool setout)
{
    if (m_isSetOut == setout) return;
    m_isSetOut = setout;
    gGame->characterDirty(this);
    gGame->notifier()->notifySimple("{C} is {D}set out", gGame->playerForCharacter(this), setout ? "":"no longer ");
}

bool Character::returnToArkham()
{
    if (!field()->backGates().isEmpty()) {
        // TODO: Let choose gate
        auto l = field()->backGates();
        shuffle_list(l);
        Gate *p = l.at(0);
        p->comeBack(this);
        //gGame->notifier()->actionExecute(this, QString("Returned to %1").arg(p->field()->name()));
        return true;
    }
    return false;
}

