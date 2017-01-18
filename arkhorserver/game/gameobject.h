#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QList>
#include <gameobjectdata.h>
#include "propertymodifier.h"

class GameAction;
class GameOption;
class GameRegistry;
class Player;
class Character;

class GameObject : public AH::Common::GameObjectData, public PropertyModifier
{
public:
    void setId(const QString &id) { m_id = id; }
    virtual GameObject *clone() = 0;

    GameObject() : m_owner(NULL), m_isInfinite(false) {}
    virtual ~GameObject() {}

    Character *owner() const { return m_owner; }
    void setOwner(Character *c) { m_owner = c; }

    virtual QString modifierId() const { return id(); }

    virtual void exhaust();
    virtual void refresh();

    virtual bool onAddToInventory(Character *c);
    virtual bool onRemoveFromInventory(Character *c) { Q_UNUSED(c) return true; }

    virtual QList<GameAction *> getActions() const = 0;
    virtual QList<GameOption *> getOptions() const = 0;

    virtual QStringList actionIds() const;
    virtual QStringList optionIds() const;

    virtual bool resolveDependencies(GameRegistry *game) = 0;
    virtual bool isFightObject();

    virtual bool equip(Player *p);
    virtual bool unequip();

    void returnToDeck();

    // For spells
    virtual bool cast(Player *p) = 0;

    virtual QList<AH::Common::PropertyModificationData> getModificationData() const;

    //virtual PropertyModificationList getModifications() const = 0;

    bool isInfinite() const { return m_isInfinite; }
    void makeInfinite();

protected:
    Character *m_owner;
    bool m_isInfinite;
};

/*
class ExhaustableGameObject : public GameObject
{
public:
    ExhaustableGameObject() : m_isExhausted(false) {}

    virtual bool isExhausted() const { return m_isExhausted; }
    virtual void exhaust() { m_isExhausted = true; }
    virtual void refresh() { m_isExhausted = false; }

protected:
    bool m_isExhausted;
};
*/

#endif // GAMEOBJECT_H
