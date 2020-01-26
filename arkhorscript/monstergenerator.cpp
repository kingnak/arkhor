#include "monstergenerator.h"

namespace AHS {

bool MonsterGenerator::generate(const ClassGenerator::ClassDef &cls)
{
    outputClassComment(cls);
    outputCreateMonsterStart(cls);
    if (!outputAttributes(cls)) return false;
    outputCreateEnd(cls);
    outputRegisterConstant(cls);
    outputRegisterMonster(cls);
    return true;
}

void MonsterGenerator::outputCreateMonsterStart(const ClassGenerator::ClassDef &cls)
{
    outputCreateStart("Monster", cls);
}

void MonsterGenerator::outputRegisterMonster(const ClassGenerator::ClassDef &cls)
{
    outputRegisterMulti("Monster", cls);
}

QList<ClassGenerator::AttributeDesc> MonsterGenerator::getAttributes()
{
    return QList<AttributeDesc>()
            << AttributeDesc("tid", AttributeDesc::R_Predefined, AttributeDesc::H_TID, AttributeDesc::V_Primitive)
            << AttributeDesc("name", AttributeDesc::R_Default, AttributeDesc::H_Name, AttributeDesc::V_Primitive)
            << AttributeDesc("description", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("mythText", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("movementType", AttributeDesc::R_Required, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("awareness", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("horrorAdjustment", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("horrorDamage", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("combatAdjustment", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("combatDamage", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("toughness", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("onDamage", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
            << AttributeDesc("onMove", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("attributes", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Primitive | AttributeDesc::V_Array)
            << AttributeDesc("dimension", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("modifications", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Complex | AttributeDesc::V_Function)
            << AttributeDesc("onDefeat", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
            << AttributeDesc("onEvade", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
            << AttributeDesc("onFlee", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
            << AttributeDesc("onHorror", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
               ;
}

bool MonsterGenerator::outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr)
{
    if (desc.name == "movementType") {
        return outputEnumValue("Constants.Movement", attr, cls);
    }
    if (desc.name == "onDamage") {
        return outputFunction(attr, cls, "type, chr");
    }
    if (desc.name == "onDefeat" || desc.name == "onEvade" || desc.name == "onFlee" || desc.name == "onHorror") {
        return outputFunction(attr, cls, "chr");
    }
    if (desc.name == "attributes") {
        return outputEnumValueArray("Constants.Monster", attr, cls);
    }
    if (desc.name == "modifications") {
        return outputModifications(attr, cls);
    }
    if (desc.name == "dimension") {
        return outputEnumValue("Constants.Dimension", attr, cls);
    }
    Q_ASSERT_X(false, "Monster Generator", qPrintable(QString("Special Attribute '%1' not handled").arg(desc.name)));
    return true;
}

}
