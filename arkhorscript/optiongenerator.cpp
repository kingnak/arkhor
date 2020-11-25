#include "optiongenerator.h"

namespace AHS
{

bool OptionGenerator::generate(const ClassGenerator::ClassDef &cls)
{
    outputClassComment(cls);
    outputCreateOptionStart(cls);
    if (!outputAttributes(cls)) return false;
    outputCreateEnd(cls);
    outputRegisterConstant(cls);
    outputRegisterOption(cls);
    return true;
}

void OptionGenerator::outputCreateOptionStart(const ClassGenerator::ClassDef &cls)
{
    outputCreateStart("Option", cls);
}

void OptionGenerator::outputRegisterOption(const ClassGenerator::ClassDef &cls)
{
    outputRegisterSingle("Option", cls);
}

QList<ClassGenerator::AttributeDesc> OptionGenerator::getAttributes()
{
    return QList<AttributeDesc>()
            << AttributeDesc("name", AttributeDesc::R_Default, AttributeDesc::H_Name, AttributeDesc::V_Primitive)
            << AttributeDesc("id", AttributeDesc::R_Predefined, AttributeDesc::H_ID, AttributeDesc::V_Primitive)
            << AttributeDesc("actionId", AttributeDesc::R_Required, AttributeDesc::H_IDRef, AttributeDesc::V_Primitive)
            << AttributeDesc("isAvailable", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Primitive | AttributeDesc::V_Function)
            << AttributeDesc("continueType", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("chooseType", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("costs", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Complex)
            << AttributeDesc("baseProperty", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
               ;
}

bool OptionGenerator::outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr)
{
    if (desc.name == "isAvailable") {
        if (attr.type == ArkhorScriptParser::Primitive) {
            return outputPrimitive(AttrDef("isAvailable", ArkhorScriptParser::Primitive, attr.content), cls);
        } else {
            return outputFunction(attr, cls);
        }
    }

    if (desc.name == "continueType") {
        return outputEnumValue("Constants.Option", attr, cls);
    }

    if (desc.name == "chooseType") {
        return outputEnumValue("Constants.Option", attr, cls);
    }

    if (desc.name == "costs") {
        return outputCosts(attr, cls);
    }

    if (desc.name == "baseProperty") {
        return outputEnumValue("Constants.Mods", attr, cls);
    }

    Q_ASSERT_X(false, "Option Generator", qPrintable(QString("Special Attribute '%1' not handled").arg(desc.name)));
    return false;
}



}
