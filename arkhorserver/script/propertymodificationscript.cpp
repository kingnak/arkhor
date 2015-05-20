#include "propertymodificationscript.h"
#include "gamescript.h"
#include <QScriptContext>

PropertyModificationScript::PropertyModificationScript()
{
}

bool PropertyModificationScript::parsePropertyModificationList(const PropertyModifier *modifier, QScriptValue propModList, PropertyModificationList &out)
{
    QScriptValueList lst;
    if (propModList.isArray()) {
        lst = GameScript::array2list(propModList);
    } else if (propModList.isObject()) {
        lst << propModList;
    } else if (!propModList.isValid() || propModList.isNull()){
        out = PropertyModificationList();
        return true;
    } else {
        return false;
    }

    out = PropertyModificationList();
    foreach (QScriptValue v, lst) {
        PropertyModification m(NULL, PropertyValue::NoProperty, 0);
        if (!parsePropertyModification(modifier, v, m)) {
            return false;
        }
        out << m;
    }

    return true;
}

bool PropertyModificationScript::parsePropertyModification(const PropertyModifier *modifier, QScriptValue propMod, PropertyModification &out)
{
    if (!propMod.isObject()) {
        return false;
    }

    PropertyValue::Property prop = static_cast<PropertyValue::Property> (propMod.property("property").toUInt32());
    int val = propMod.property("value").toInt32();

    out = PropertyModification(modifier, prop, val);
    return true;
}
