#ifndef PROPERTYMODIFIER_H
#define PROPERTYMODIFIER_H

#include <QList>
#include "propertyvalue.h"

class ModifiedPropertyValue;
class PropertyModifier;

class PropertyModification : public AH::Common::PropertyModificationData
{
public:
    PropertyModification(const PropertyModifier *modifier, PropertyValue::Property prop, int mod)
        : AH::Common::PropertyModificationData(prop, mod), m_modifier(modifier)
    {
    }

    const PropertyModifier *getModifier() const { return m_modifier; }
    int modify(int base) const { return base + m_mod; }
    //virtual QString modifierId() const;
private:
    const PropertyModifier *m_modifier;

};

class PropertyModificationList : public QList<PropertyModification>
{
public:
    PropertyModificationList filtered(PropertyValue::Property prop);
    int apply(int value);
    PropertyModificationList operator + (const PropertyModificationList &o) const {
        PropertyModificationList ret;
        ret.append(*this);
        ret.append(o);
        return ret;
    }

    QList<AH::Common::PropertyModificationData> toPropertyModificationDataList() const;
};

class ModifiedPropertyValue
{
public:
    ModifiedPropertyValue() : m_prop(PropertyValue::NoProperty, 0), m_finalVal(0) {}
    ModifiedPropertyValue(PropertyValue v, int finalVal, PropertyModificationList mods)
        : m_prop(v), m_finalVal(finalVal), m_mods(mods) {}

    PropertyValue::Property property() const { return m_prop.property(); }
    int base() const { return m_prop.value(); }
    int finalVal() const { return m_finalVal; }
    PropertyModificationList modifiers() const { return m_mods; }

    AH::Common::ModifiedPropertyValueData toModifiedPropertyValueData() const;

private:
    PropertyValue m_prop;
    int m_finalVal;
    PropertyModificationList m_mods;
};

class PropertyModifier
{
public:
    virtual QString modifierId() const = 0;
    virtual PropertyModificationList getModifications() = 0;
};

#endif // PROPERTYMODIFIER_H
