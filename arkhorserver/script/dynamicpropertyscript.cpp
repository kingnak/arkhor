#include "dynamicpropertyscript.h"
#include "gamescript.h"
#include <QScriptValueIterator>
#include <QDynamicPropertyChangeEvent>

DynamicPropertyScript::DynamicPropertyScript() = default;

void DynamicPropertyScript::setObject(const QScriptValue &v)
{
    m_this = v;
}

QString DynamicPropertyScript::display() const
{
    if (m_toString.isFunction()) {
        QScriptValue ts = m_toString;
        if (gGameScript->isGameThread()) {
            m_oldDynFuncValue = gGameScript->call(GameScript::F_Display, ts, m_this).toString();
        }
        return m_oldDynFuncValue;
    }
    return m_val.toString();
}

bool DynamicPropertyScript::createDynamicProperties(const QScriptValue &v, DynamicScriptableObject *o)
{
    if (!v.isObject()) {
        return false;
    }

    QScriptValueIterator it(v);
    while (it.hasNext()) {
        it.next();
        if (it.flags() & QScriptValue::SkipInEnumeration)
            continue;

        QString name = it.name();
        QScriptValue val = it.value();
        DynamicPropertyScript dp;
        parseDynamicProperty(name, val, dp);
        o->addDynamicProperty(dp);
    }

    return true;
}

bool DynamicPropertyScript::parseDynamicProperty(const QString &name, const QScriptValue &v, DynamicPropertyScript &o)
{
    o.m_propertyName = name.toUtf8();
    if (v.isObject()) {
        QScriptValue n = v.property("display");
        if (n.isValid()) {
            o.m_name = n.toString();
        } else {
            o.m_name = name;
        }

        QScriptValue ts = v.property("onDisplay");
        if (ts.isFunction()) {
            o.m_toString = ts;
        }

        o.m_val = v.property("value").toVariant();
    } else {
        o.m_name = name;
        o.m_val = v.toVariant();
    }

    return true;
}

void DynamicScriptableObject::addDynamicProperty(const DynamicPropertyScript &prop)
{
    m_dynamicProperties.insert(prop.propertyName(), prop);
    setProperty(prop.propertyName(), prop.value().isValid() ? prop.value() : "");
}

QList<AH::Common::DynamicPropertyData> DynamicScriptableObject::getDynamicPropertyDataList() const
{
    QList<AH::Common::DynamicPropertyData> ret;
    for (const auto &prop : m_dynamicProperties) {
        QVariant v = property(prop.propertyName());

        //QString s = v.toString();

        AH::Common::DynamicPropertyData c(prop.name());
        c.setValue(v);
        c.setDisplay(prop.display());
        ret << c;
    }
    return ret;
}

void DynamicScriptableObject::clonePropertiesInto(DynamicScriptableObject *other)
{
    for (const auto &prop : m_dynamicProperties) {
        other->addDynamicProperty(prop);
    }
}

void DynamicScriptableObject::resolveDependencies(const QScriptValue &thisObj)
{
    /*
    for (auto &prop : m_dynamicProperties.values()) {
        prop.setObject(thisObj);
    }
    */
    for (QMap<QByteArray, DynamicPropertyScript>::iterator it = m_dynamicProperties.begin(); it != m_dynamicProperties.end(); ++it) {
        it.value().setObject(thisObj);
        // Force calling display function in main thread
        it.value().display();
    }
}

bool DynamicScriptableObject::event(QEvent *eve)
{
    if (eve->type() == QEvent::DynamicPropertyChange) {
        QDynamicPropertyChangeEvent *dynPropChange = static_cast<QDynamicPropertyChangeEvent *> (eve);
        if (m_dynamicProperties.contains(dynPropChange->propertyName())) {
            QVariant v = this->property(dynPropChange->propertyName());
            m_dynamicProperties[dynPropChange->propertyName()].setValue(v);
            // Force recalculation in main thead
            m_dynamicProperties[dynPropChange->propertyName()].display();
            this->dynamicPropertyChanged();
            return true;
        }
    }
    return false;
}
