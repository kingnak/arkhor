#include "propertymodificationscript.h"
#include "gamescript.h"
#include <QScriptContext>

PropertyModificationScript::PropertyModificationScript()
:   m_engine(nullptr)
{
}

bool PropertyModificationScript::parsePropertyModificationList(const PropertyModifier *modifier, QScriptValue propModList, PropertyModificationList &out)
{
    QScriptValueList lst;
    if (propModList.isArray()) {
        lst = GameScript::array2list(propModList);
    } else if (propModList.isObject()) {
        lst << propModList;
    } else if (!propModList.isValid() || propModList.isNull() || propModList.isUndefined()){
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
    //int val = propMod.property("value").toInt32();
    QString mod = propMod.property("value").toString().trimmed();
    PropertyModification::ModificationType type;
    int skip = 1;
    if (mod.startsWith("^/")) {
        type = PropertyModification::DividingUp;
        skip = 2;
    } else if (mod.startsWith("/")) {
        type = PropertyModification::DividingDown;
    } else if (mod.startsWith("*")) {
        type = PropertyModification::Multiplicative;
    } else if (mod.startsWith("=")) {
        type = PropertyModification::Setting;
    } else if (mod.startsWith("-")) {
        type = PropertyModification::Additive;
        skip = 0;
    } else if (mod.startsWith("+")) {
        type = PropertyModification::Additive;
    } else if (mod.startsWith("|")) {
        type = PropertyModification::Oring;
    } else if (mod.at(0).isDigit()) {
        type = PropertyModification::Additive;
        skip = 0;
    } else {
        return false;
    }

    bool ok;
    mod = mod.mid(skip);
    int val = mod.toInt(&ok);
    if (!ok) return false;

    out = PropertyModification(modifier, prop, val, type);
    return true;
}
