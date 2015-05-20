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


