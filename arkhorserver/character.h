#ifndef CHARACTERAAAA_H
#define CHARACTERAAAA_H

#include <ahglobal.h>
#include "attributeslider.h"
#include <QList>
#include "game/game.h"
#include "game/propertymodifier.h"
#include "game/investigator.h"
#include <characterdata.h>
#include <cost.h>

class GameField;
class Monster;
class Gate;

class GameAction;
class GameOption;
class GameObject;

class Character : public AH::Common::CharacterData
{
public:
    Character(Investigator *i);

    CharacterData *data();

    PropertyModificationList getPropertyModifiers() const;

    QList<GameAction *> getActions(AH::GamePhase phase);
    QList<GameOption *> getOptions(AH::GamePhase phase);
    QList<GameObject *> &inventory();
    Investigator *investigator() { return m_investigator; }
    AH::Common::InvestigatorData investigatorData() const { return *m_investigator->data(); }

    void setDirty(bool dirty = true) { m_dirty = dirty; }
    bool isDirty() const { return m_dirty; }

    // Attributes
    QList<AttributeSlider> getFocusAttributes() const;
    QList<AttributeSlider> &getModifyableFocusAttributes();
    int getAttributeValue(AH::Attribute attr) const;
    // FOR TESTING ONLY:
    //void setFocusAttributes(QList<AttributeSlider> attributes) { m_sliders = attributes; }

    // Fixed values
    int getMaximumFocus() const { return m_maxFocus; }
    void addClue(int amount) { m_clues += amount; gGame->characterDirty(this); }
    void addMoney(int amount) { m_money += amount; gGame->characterDirty(this); }
    void setMovementAmount(int amount) { m_movementPoints = amount; gGame->characterDirty(this); }
    void setFocusAmount(int amount) { m_curFocus = amount; gGame->characterDirty(this); }

    void damageStamina(int amount);
    void damageSanity(int amount);
    bool commitDamage();
    void addStamina(int amount);
    void addSanity(int amount);
    void restoreStamina();
    void restoreSanity();

    bool returnToArkham();

    // Items
    void addMonsterMarker(Monster *m) { if (!m) return; m_monsterMarkers.append(m); gGame->characterDirty(this); }
    void addGateMarker(Gate *p) { if (!p) return; m_gateMarkers.append(p); gGame->characterDirty(this); }

    // Game state
    const Gate *getExploredGate() const;
    void setExploredGate(const Gate *p) { m_explorededGate = p; gGame->characterDirty(this); }

    void setDelayed(bool delay) { m_delayed = delay; gGame->characterDirty(this); }
    void setSetout(bool setout) { m_isSetOut = setout; gGame->characterDirty(this); }
    void setOtherWoldPhase(AH::OtherWorldPhase owP) { m_owPhase = owP; gGame->characterDirty(this); }

    GameField *field() { return m_field; }
    void setField(GameField *f) { m_field = f; gGame->characterDirty(this); }

    // Payment
    bool canPay(const AH::Common::Cost &cost) const;
    bool canPay(const AH::Common::CostList &cost) const;
    bool pay(const AH::Common::CostList &cost);

    void losePossessions();
    void unconscious();
    void insane();
    void devour();
    void lostInSpaceAndTime();

protected:
    void instantiateFromInvestigator();

protected:
    Investigator *m_investigator;

    bool m_dirty;

    QList<GameObject *> m_inventory;
    QList<AttributeSlider> m_sliders;
    GameField *m_field;
    int m_maxFocus;

    int m_curDmgStamina;
    int m_curDmgSanity;

    const Gate *m_explorededGate;
    //AH::OtherWorldPhase m_owPhase;

    QList<Gate *> m_gateMarkers;
    QList<Monster *> m_monsterMarkers;
};

#endif // CHARACTER_H
