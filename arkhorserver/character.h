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

    CharacterData *data() override;

    PropertyModificationList getPropertyModifiers() const;

    int maxStamina() const override;
    int maxSanity() const override;
    int baseMaxStamina() const { return m_maxStamina; }
    int baseMaxSanity() const { return m_maxSanity; }

    QList<GameAction *> getActions(AH::GamePhase phase);
    QList<GameOption *> getOptions(AH::GamePhase phase);
    QList<GameObject *> &inventory();
    virtual void addToInventory(GameObject *obj);
    virtual void removeFromInventory(GameObject *obj);

    Investigator *investigator() { return m_investigator; }
    AH::Common::InvestigatorData investigatorData() const override { return *m_investigator->data(); }

    void setDirty(bool dirty = true) { m_dirty = dirty; }
    bool isDirty() const { return m_dirty; }

    GameObject *getObjectFromInventory(const QString &typeId) const;

    // Attributes
    QList<AttributeSlider> getFocusAttributes() const;
    QList<AttributeSlider> &getModifyableFocusAttributes();
    int getAttributeValue(AH::Attribute attr) const;
    // FOR TESTING ONLY:
    //void setFocusAttributes(QList<AttributeSlider> attributes) { m_sliders = attributes; }

    // Fixed values
    int getMaximumFocus() const { return m_maxFocus; }
    void addClue(int amount);
    void loseClue(int amount);
    void addMoney(int amount);
    void setMovementAmount(int amount) { m_movementPoints = amount; gGame->characterDirty(this); }
    void addMovementPoint(int amount) { m_movementPoints += amount; gGame->characterDirty(this); }
    void setFocusAmount(int amount) { m_curFocus = amount; gGame->characterDirty(this); }
    void loseMoney(int amount);

    void damageStamina(int amount);
    void damageSanity(int amount);
    bool commitDamage();
    void addStamina(int amount);
    void addSanity(int amount);
    void restoreStamina();
    void restoreSanity();

    int freeHands() const;
    //void preventDamageStamina(int amount);
    //void preventDamageSanity(int amount);


    // Items
    void addMonsterTrophy(Monster *m);
    void addGateTrophy(Gate *p);

    // Game state
    const Gate *getExploredGate() const { return m_explorededGate; }
    void setExploredGate(const Gate *p);

    void setDelayed(bool delay);
    void setSetout(bool setout);
    void setOtherWoldPhase(AH::OtherWorldPhase owP) { m_owPhase = owP; gGame->characterDirty(this); }

    GameField *field() { return m_field; }
    void setField(GameField *f) { m_field = f; gGame->characterDirty(this); }

    // Payment
    bool canPay(const AH::Common::Cost &cost) const;
    bool canPay(const AH::Common::CostList &cost) const;
    bool pay(const AH::Common::Cost &cost, const QString &sourceId = {}, const QString &description = {});
    bool pay(const AH::Common::CostList &cost);

    void loseHalfClues();
    void loseHalfPossessions(const QString &sourceId = QString::null);
    void losePossessions(int count, const QString &sourceId = QString::null);
    void arrest();
    void unconscious();
    void insane();
    void devour();
    void lostInSpaceAndTime();
    bool returnToArkham();

    virtual bool onUnconscious() { return true; }
    virtual bool onInsane() { return true; }
    virtual bool onLostInSpaceAndTime() { return true; }
    virtual bool onOpenGate(Gate *g) { Q_UNUSED(g) return true; }
    virtual bool onAppearMonster(Monster *m) { Q_UNUSED(m) return true; }

protected:
    virtual void instantiateFromInvestigator();

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

    friend class Investigator;
};

#endif // CHARACTER_H
