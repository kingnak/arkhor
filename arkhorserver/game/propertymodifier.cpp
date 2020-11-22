#include "propertymodifier.h"

PropertyModificationList PropertyModificationList::filtered(PropertyValue::Property prop)
{
    PropertyModificationList ret;
    for (auto p : *this) {
        if (p.affectedProperty() == prop) {
            ret << p;
        }
    }
    return ret;
}

struct SortByType {
    bool operator() (const PropertyModification &l, const PropertyModification &r) {
        return l.type() < r.type();
    }
};

int PropertyModificationList::apply(int value) const
{
    QList<PropertyModification> lst = *this;
    qSort(lst.begin(), lst.end(), SortByType());
    for (PropertyModification p : lst) {
        value = p.modify(value);
    }
    return value;
}

QList<AH::Common::PropertyModificationData> PropertyModificationList::toPropertyModificationDataList() const
{
    QList<AH::Common::PropertyModificationData> ret;
    ret.reserve(this->size());
    for (auto mod : *this) {
        ret << *(mod.data());
    }
    return ret;
}

AH::Common::ModifiedPropertyValueData ModifiedPropertyValue::toModifiedPropertyValueData() const
{
    return AH::Common::ModifiedPropertyValueData(m_prop, finalVal(), m_mods.toPropertyModificationDataList());
}

bool PropertyModification::operator ==(const PropertyModification &o) const
{
    return (o.m_mod == m_mod
            && o.m_prop == m_prop
            && o.m_type == m_type);
}

AH::Common::PropertyModificationData *PropertyModification::data()
{
    m_modifierId = (m_modifier) ? m_modifier->modifierId() : m_modifierId;
    return AH::Common::PropertyModificationData::data();
}

int PropertyModification::modify(int base) const
{
    switch (m_type) {
    case Additive: return base + m_mod;
    case Multiplicative: return base * m_mod;
    case DividingUp: return (base + m_mod-1)/m_mod;
    case DividingDown: return base/m_mod;
    case Setting: return m_mod;
    case Oring: return base | m_mod;
    }
    Q_ASSERT_X(false, "PropertyModification::modify", qPrintable(QString("Unknown modification type %1").arg(m_type)));
    return 0;
}
