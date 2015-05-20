#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QList>
#include <gameobjectdata.h>
#include "propertymodifier.h"

class GameAction;
class GameOption;
class Game;

class GameObject : public AH::Common::GameObjectData, public PropertyModifier
{
public:
    GameObject() {}
    virtual ~GameObject() {}

    virtual void exhaust() { if (m_bExhaustable) m_bIsExhausted = true; }
    virtual void refresh() { if (m_bExhaustable) m_bIsExhausted = false; }

    virtual QList<GameAction *> getActions() const = 0;
    virtual QList<GameOption *> getOptions() const = 0;

    virtual QStringList actionIds() const;
    virtual QStringList optionIds() const;

    virtual bool resolveDependencies(const Game *game) = 0;

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
