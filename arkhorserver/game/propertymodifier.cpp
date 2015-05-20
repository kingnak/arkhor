#include "propertymodifier.h"

PropertyModificationList PropertyModificationList::filtered(PropertyValue::Property prop)
{
    PropertyModificationList ret;
    foreach (PropertyModification p, *this) {
        if (p.affectedProperty() == prop) {
            ret << p;
        }
    }
    return ret;
}

int PropertyModificationList::apply(int value)
{
    foreach (PropertyModification p, *this) {
        value = p.modify(value);
    }
    return value;
}

QList<AH::Common::PropertyModificationData> PropertyModificationList::toPropertyModificationDataList() const
{
    QList<AH::Common::PropertyModificationData> ret;
    ret.reserve(this->size());
    foreach (PropertyModification mod, *this) {
        ret << mod;
    }
    return ret;
}

AH::Common::ModifiedPropertyValueData ModifiedPropertyValue::toModifiedPropertyValueData() const
{
    return AH::Common::ModifiedPropertyValueData(m_prop, m_finalVal, m_mods.toPropertyModificationDataList());
}
