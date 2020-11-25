#ifndef PROPERTYMODIFIER_H
#define PROPERTYMODIFIER_H

#include <QList>
#include "propertyvalue.h"

class ModifiedPropertyValue;
class PropertyModifier;

class PropertyModification : public AH::Common::PropertyModificationData
{
public:
    PropertyModification(const PropertyModifier *modifier, PropertyValue::Property prop, int mod, ModificationType type = Additive)
        : AH::Common::PropertyModificationData(prop, mod, type), m_modifier(modifier)
    {
    }

    bool operator == (const PropertyModification &o) const;

    PropertyModificationData *data() override;

    const PropertyModifier *getModifier() const { return m_modifier; }
    int modify(int base) const;
    //virtual QString modifierId() const;
private:
    const PropertyModifier *m_modifier;

};

class PropertyModificationList : public QList<PropertyModification>
{
public:
    PropertyModificationList filtered(PropertyValue::Property prop);
    int apply(int value) const;
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
    ModifiedPropertyValue(PropertyValue v, PropertyModificationList mods)
        : m_prop(v), m_mods(mods) {
        m_finalVal = m_mods.apply(m_prop.value());
    }

    PropertyValue::Property property() const { return m_prop.property(); }
    int base() const { return m_prop.value(); }
    int finalVal() const { return m_finalVal; }
    PropertyModificationList modifiers() const { return m_mods; }
    void overrideFinalValue(int value) { m_finalVal = value; }

    AH::Common::ModifiedPropertyValueData toModifiedPropertyValueData() const;

private:
    PropertyValue m_prop;
    PropertyModificationList m_mods;
    int m_finalVal;
};

class PropertyModifier
{
public:
    virtual ~PropertyModifier() = default;
    virtual QString modifierId() const = 0;
    virtual PropertyModificationList getModifications() const = 0;
};

#endif // PROPERTYMODIFIER_H
