#include "quickoptiongenerator.h"

#include <QTextStream>

namespace AHS
{

bool QuickOptionGenerator::generate(const ClassGenerator::ClassDef &cls)
{
    outputClassComment(cls);
    m_out << "var " << getJSVariableName(cls) << " = game.quickOption({\n";
    if (!outputAttributes(cls)) return false;
    outputCreateEnd(cls);
    outputRegisterConstant(cls);
    // game.quickOption automatically registers the option, no need for this:
    //outputRegisterOption(cls);
    return true;
}

QList<ClassGenerator::AttributeDesc> QuickOptionGenerator::getAttributes()
{
    QList<AttributeDesc> ret = OptionGenerator::getAttributes()
            << AttributeDesc("description", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("phases", AttributeDesc::R_Required, AttributeDesc::H_Special, AttributeDesc::V_Array | AttributeDesc::V_Primitive)
            << AttributeDesc("activate", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Function)
               ;

    // Filter out id and actionId Attribute!
    QList<AttributeDesc>::iterator it = ret.begin();
    while (it != ret.end()) {
        if (it->name == "actionId") {
            it = ret.erase(it);
        } else {
            ++it;
        }
    }
    return ret;
}

bool QuickOptionGenerator::outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr)
{
    if (desc.name == "phases") {
        return outputEnumValueArray("Constants.GamePhases", attr, cls);
    }
    return OptionGenerator::outputSpecialAttribute(desc, cls, attr);
}

///////////////////////////////////////////////////////

QList<ClassGenerator::AttributeDesc> ArkhamEncounterOptionGenerator::getAttributes()
{
    QList<AttributeDesc> ret = QuickOptionGenerator::getAttributes();
    // make phase predefined
    QList<AttributeDesc>::iterator it = ret.begin();
    while (it != ret.end()) {
        if (it->name == "phases") {
            it->reqType = AttributeDesc::R_Predefined;
            break;
        } else {
            ++it;
        }
    }
    return ret;
}

bool ArkhamEncounterOptionGenerator::outputDefaultAttribute(ClassGenerator::AttributeDesc desc, const ClassGenerator::ClassDef &cls)
{
    if (desc.name == "phases") {
        return outputAttribute(cls, AttrDef("phases", ArkhorScriptParser::EnumValue, "ArkhamEncountery"), true);
    }
    return QuickOptionGenerator::outputDefaultAttribute(desc, cls);
}

///////////////////////////////////////////////////////

QList<ClassGenerator::AttributeDesc> OtherWorldEncounterOptionGenerator::getAttributes()
{
    QList<AttributeDesc> ret = QuickOptionGenerator::getAttributes();
    // make phase predefined
    QList<AttributeDesc>::iterator it = ret.begin();
    while (it != ret.end()) {
        if (it->name == "phases") {
            it->reqType = AttributeDesc::R_Predefined;
            break;
        } else {
            ++it;
        }
    }
    return ret;
}

bool OtherWorldEncounterOptionGenerator::outputDefaultAttribute(ClassGenerator::AttributeDesc desc, const ClassGenerator::ClassDef &cls)
{
    if (desc.name == "phases") {
        return outputAttribute(cls, AttrDef("phases", ArkhorScriptParser::EnumValue, "OtherWorldEncountery"), true);
    }
    return QuickOptionGenerator::outputDefaultAttribute(desc, cls);
}


}
