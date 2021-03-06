﻿#include "mythosgenerator.h"
#include <QTextStream>

namespace AHS
{

bool MythosGenerator::generate(const ClassGenerator::ClassDef &cls)
{
    outputClassComment(cls);
    outputCreateMythosStart(cls);
    if (!outputAttributes(cls)) return false;
    outputCreateEnd(cls);
    outputRegisterConstant(cls);
    outputRegisterMythos(cls);
    return true;
}

void MythosGenerator::outputCreateMythosStart(const ClassGenerator::ClassDef &cls)
{
    outputCreateStart("MythosCard", cls);
}

void MythosGenerator::outputRegisterMythos(const ClassGenerator::ClassDef &cls)
{
    outputRegisterSingle("MythosCard", cls);
}

QList<ClassGenerator::AttributeDesc> MythosGenerator::getAttributes()
{
    return QList<AttributeDesc>()
            << AttributeDesc("id", AttributeDesc::R_Predefined, AttributeDesc::H_ID, AttributeDesc::V_Primitive)
            << AttributeDesc("type", AttributeDesc::R_Predefined, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("name", AttributeDesc::R_Default, AttributeDesc::H_Name, AttributeDesc::V_Primitive)
            << AttributeDesc("description", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("shortDescription", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("properties", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
               // Optional for random:
            << AttributeDesc("clueField", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("gateField", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("moveBlack", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Array)
            << AttributeDesc("moveWhite", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Array)
               ;
}

bool MythosGenerator::outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr)
{
    if (desc.name == "clueField" || desc.name == "gateField") {
        return outputEnumValue("Constants.Fields", attr, cls);
    }
    if (desc.name == "moveBlack" || desc.name == "moveWhite") {
        return outputEnumValue("Constants.Dimension", attr, cls);
    }
    if (desc.name == "type") {
        return outputEnumValue("Constants.Mythos", attr, cls);
    }
    Q_ASSERT_X(false, "Mythos Generator", qPrintable(QString("Special Attribute '%1' not handled").arg(desc.name)));
    return false;
}

/////////////////////////////////////////////

QList<ClassGenerator::AttributeDesc> HeadlineGenerator::getAttributes()
{
    return MythosGenerator::getAttributes()
            << AttributeDesc("executeHeadline", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Function)
               ;
}

bool HeadlineGenerator::outputDefaultAttribute(const ClassGenerator::AttributeDesc &desc, const ClassGenerator::ClassDef &cls)
{
    if (desc.name == "type") {
        return outputAttribute(cls, AttrDef("type", ArkhorScriptParser::EnumValue, "Headline"), true);
    }
    return MythosGenerator::outputDefaultAttribute(desc, cls);
}

//////////////////////////////////////////////

QList<ClassGenerator::AttributeDesc> EnvironmentGenerator::getAttributes()
{
    return MythosGenerator::getAttributes()
            << AttributeDesc("environmentFieldOptionId", AttributeDesc::R_Optional, AttributeDesc::H_IDRef, AttributeDesc::V_Primitive)
            << AttributeDesc("environmentField", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("environmentType", AttributeDesc::R_Required, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("environmentModifications", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Complex)
            << AttributeDesc("monsterModifications", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Complex)
            << AttributeDesc("monsterMoveModifications", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Complex)
            << AttributeDesc("activateEnvironment", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("onEndMovement", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("onDefeatMonster", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
            << AttributeDesc("onAppearMonster", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
               ;
}

bool EnvironmentGenerator::outputSpecialAttribute(const ClassGenerator::AttributeDesc &desc, const ClassGenerator::ClassDef &cls, const ClassGenerator::AttrDef &attr)
{
    if (desc.name == "environmentType") {
        return outputEnumValue("Constants.EnvironmentType", attr, cls);
    }
    if (desc.name == "environmentField") {
        return outputEnumValue("Constants.Fields", attr, cls);
    }
    if (desc.name == "environmentModifications") {
        return outputModifications(attr, cls);
    }
    if (desc.name == "monsterModifications") {
        return outputMonsterModifications(attr, cls);
    }
    if (desc.name == "monsterMoveModifications") {
        return outputMonsterMoveModifications(attr, cls);
    }
    if (desc.name == "onDefeatMonster") {
        return outputFunction(attr, cls, "c, m");
    }
    if (desc.name == "onAppearMonster") {
        return outputFunction(attr, cls, "f,m");
    }
    return MythosGenerator::outputSpecialAttribute(desc, cls, attr);
}

bool EnvironmentGenerator::outputDefaultAttribute(const ClassGenerator::AttributeDesc &desc, const ClassGenerator::ClassDef &cls)
{
    if (desc.name == "type") {
        return outputAttribute(cls, AttrDef("type", ArkhorScriptParser::EnumValue, "Environment"), true);
    }
    return MythosGenerator::outputDefaultAttribute(desc, cls);
}

//////////////////////////////////////////////

QList<ClassGenerator::AttributeDesc> RumorGenerator::getAttributes()
{
    return MythosGenerator::getAttributes()
            << AttributeDesc("rumorFieldOptionId", AttributeDesc::R_Required, AttributeDesc::H_IDRef, AttributeDesc::V_Primitive)
            << AttributeDesc("rumorField", AttributeDesc::R_Required, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("setupRumor", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("teardownRumor", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("onMythos", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("onPass", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("onFail", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Function)
               ;
}

bool RumorGenerator::outputSpecialAttribute(const ClassGenerator::AttributeDesc &desc, const ClassGenerator::ClassDef &cls, const ClassGenerator::AttrDef &attr)
{
    if (desc.name == "rumorField") {
        return outputEnumValue("Constants.Fields", attr, cls);
    }
    return MythosGenerator::outputSpecialAttribute(desc, cls, attr);
}

bool RumorGenerator::outputDefaultAttribute(const ClassGenerator::AttributeDesc &desc, const ClassGenerator::ClassDef &cls)
{
    if (desc.name == "type") {
        return outputAttribute(cls, AttrDef("type", ArkhorScriptParser::EnumValue, "Rumor"), true);
    }
    return MythosGenerator::outputDefaultAttribute(desc, cls);
}


}
