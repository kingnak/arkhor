#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QList>
#include <gameobjectdata.h>
#include "propertymodifier.h"

class GameAction;
class GameOption;
class GameRegistry;
class Player;

class GameObject : public AH::Common::GameObjectData, public PropertyModifier
{
public:
    void setId(const QString &id) { m_id = id; }
    virtual GameObject *clone() = 0;

    GameObject() {}
    virtual ~GameObject() {}

    virtual QString modifierId() const { return typeId(); }

    virtual void exhaust() { if (m_bExhaustable) m_bIsExhausted = true; }
    virtual void refresh() { if (m_bExhaustable) m_bIsExhausted = false; }

    virtual QList<GameAction *> getActions() const = 0;
    virtual QList<GameOption *> getOptions() const = 0;

    virtual QStringList actionIds() const;
    virtual QStringList optionIds() const;

    virtual bool resolveDependencies(GameRegistry *game) = 0;
    virtual bool isFightObject();

    virtual bool equip(Player *p);
    virtual bool unequip();

    // For spells
    virtual bool cast(Player *p) = 0;

    //virtual PropertyModificationList getModifications() const = 0;
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
