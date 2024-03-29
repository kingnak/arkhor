#include "spellgenerator.h"

namespace AHS {

QList<ClassGenerator::AttributeDesc> SpellGenerator::getAttributes()
{
    return ObjectGenerator::getAttributes()
            << AttributeDesc("castTarget", AttributeDesc::R_Default, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("castCost", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("castAdjustment", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("exhaustable", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("cast", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("castIsAvailable", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("castChooseType", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("castContinueType", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("castPhases", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Array | AttributeDesc::V_Primitive)
            << AttributeDesc("hands", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
               ;
}

bool SpellGenerator::outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr)
{
    /*
    if (desc.name == "cast") {
        return outputFunction(attr, cls);
    }
    */
    if (desc.name == "castChooseType") {
        return outputEnumValue("Constants.Option", attr, cls);
    }
    if (desc.name == "castContinueType") {
        return outputEnumValue("Constants.Option", attr, cls);
    }
    if (desc.name == "castPhases") {
        return outputEnumValueArray("Constants.GamePhases", attr, cls);
    }
    return ObjectGenerator::outputSpecialAttribute(desc, cls, attr);
}

bool SpellGenerator::outputDefaultAttribute(const ClassGenerator::AttributeDesc &desc, const ClassGenerator::ClassDef &cls)
{
    if (desc.name == "castTarget") {
        return outputAttribute(cls, AttrDef("castTarget", ArkhorScriptParser::Primitive, "1"), false);
    }
    return ObjectGenerator::outputDefaultAttribute(desc, cls);
}

}
