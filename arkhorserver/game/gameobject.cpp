#include "gameobject.h"
#include "gameaction.h"
#include "gameoption.h"
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
