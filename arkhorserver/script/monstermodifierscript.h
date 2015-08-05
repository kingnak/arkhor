#ifndef MONSTERMODIFIERSCRIPT_H
#define MONSTERMODIFIERSCRIPT_H

#include "game/monstermodifier.h"
#include <QScriptValue>

class MonsterModifierScript
{
public:
    static bool parseMonsterModifications(QScriptValue data, MonsterModifier &mod, const PropertyModifier *propMod);

};

#endif // MONSTERMODIFIERSCRIPT_H
