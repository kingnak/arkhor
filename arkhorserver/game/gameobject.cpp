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

bool GameObject::isFightObject()
{
    foreach (PropertyModification m, getModifications()) {
        switch (m.affectedProperty()) {
        case PropertyValue::Damage_General:
        case PropertyValue::Damage_Physical:
        case PropertyValue::Damage_Magical:
            return true;
        }
    }
    return false;
}
