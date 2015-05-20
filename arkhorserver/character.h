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

    PropertyModificationList getPropertyModifiers() const { return PropertyModificationList(); }

    QList<GameAction *> getActions(AH::GamePhase phase);
    QList<GameOption *> getOptions(AH::GamePhase phase);
    QList<GameObject *> &inventory();

    // Attributes
    QList<AttributeSlider> getFocusAttributes() const;
    QList<AttributeSlider> &getModifyableFocusAttributes();
    int getAttributeValue(AH::Attribute attr) const;
    // FOR TESTING ONLY:
    void setFocusAttributes(QList<AttributeSlider> attributes) { m_sliders = attributes; }

    // Fixed values
    int getMaximumFocus() const { return m_maxFocus; }
    void addClue(int amount) { m_clues += amount; }
    void setMovementAmount(int amount) { m_movementPoints = amount; }
    void setFocusAmount(int amount) { m_curFocus = amount; }

    void damageStamina(int amount);
    void damageSanity(int amount);
    void addStamina(int amount);
    void addSanity(int amount);

    // Items
    void addMonsterMakrker(Monster *m) { m_monsterMarkers.append(m); }
    void addGateMarker(Gate *p) { m_gateMarkers.append(p); }

    // Game state
    const Gate *getExploredGate() const;
    void setExploredGate(const Gate *p) { m_explorededGate = p; }

    void setDelayed(bool delay) { m_delayed = delay; }
    void setOtherWoldPhase(AH::OtherWorldPhase owP) { m_owPhase = owP; }

    GameField *field() { return m_field; }
    void setField(GameField *f) { m_field = f; }

    // Payment
    bool canPay(const AH::Common::Cost &cost) const;
    bool canPay(const AH::Common::CostList &cost) const;
    bool pay(const AH::Common::CostList &cost);

    void unconscious();
    void insane();
    void devour();

protected:
    void instantiateFromInvestigator();

protected:
    Investigator *m_investigator;

    QList<GameObject *> m_inventory;
    QList<AttributeSlider> m_sliders;
    GameField *m_field;
    int m_maxFocus;

    const Gate *m_explorededGate;
    AH::OtherWorldPhase m_owPhase;

    QList<Gate *> m_gateMarkers;
    QList<Monster *> m_monsterMarkers;
};

#endif // CHARACTER_H
