#ifndef PROPERTYVALUE_H
#define PROPERTYVALUE_H

#include <ahglobal.h>
#include <QList>
#include <propertyvaluedata.h>

class PropertyModification;

class PropertyValue : public AH::Common::PropertyValueData
{
public:
    PropertyValue(Property prop, int val) : AH::Common::PropertyValueData(prop, val) {}

    static bool isAttributeProperty(Property p);
    static bool isSkillProperty(Property p);
    static bool isCharacterProperty(Property p);
    static bool isGameProperty(Property p);

    static Property attribute2Property(AH::Attribute attr);
    static Property skill2Property(AH::Skill skill);
    static AH::Skill property2Skill(Property prop);
    static AH::Attribute property2Attribute(Property prop);
};

#endif // PROPERTYVALUE_H
