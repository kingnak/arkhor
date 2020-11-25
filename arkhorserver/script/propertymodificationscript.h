#ifndef PROPERTYMODIFICATIONSCRIPT_H
#define PROPERTYMODIFICATIONSCRIPT_H

#include "game/propertymodifier.h"
#include <QScriptValue>

class PropertyModifier;

class PropertyModificationScript
{
public:
    PropertyModificationScript();

    static bool parsePropertyModificationList(const PropertyModifier *modifier, const QScriptValue &propModList, PropertyModificationList &out);
    static bool parsePropertyModification(const PropertyModifier *modifier, const QScriptValue &propMod, PropertyModification &out);

private:
    QScriptEngine *m_engine;
};

#endif // PROPERTYMODIFICATIONSCRIPT_H
