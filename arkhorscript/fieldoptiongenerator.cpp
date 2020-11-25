#include "fieldoptiongenerator.h"

#include <QTextStream>

namespace AHS
{

bool FieldOptionGenerator::generate(const ClassGenerator::ClassDef &cls)
{
    if (QuickOptionGenerator::generate(cls)) {
        m_out << "game.addFieldOption(Constants.Fields." << m_field << ", " << getJSVariableName(cls) << ".id);\n\n";
        return true;
    }
    return false;
}

QList<ClassGenerator::AttributeDesc> FieldOptionGenerator::getAttributes()
{
    QList<ClassGenerator::AttributeDesc> ret = QuickOptionGenerator::getAttributes();

    // Filter out id and phases Attribute!
    QList<AttributeDesc>::iterator it = ret.begin();
    while (it != ret.end()) {
        if (it->name == "phases") {
            it = ret.erase(it);
        } else {
            ++it;
        }
    }
    ret << AttributeDesc("field", AttributeDesc::R_Required, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
        << AttributeDesc("phases", AttributeDesc::R_Predefined, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
               ;
    return ret;
}

bool FieldOptionGenerator::outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr)
{
    if (desc.name == "field") {
        m_field = attr.content.first;
        return outputEnumValue("Constants.Fields", attr, cls);
    }
    return QuickOptionGenerator::outputSpecialAttribute(desc, cls, attr);
}

bool FieldOptionGenerator::outputDefaultAttribute(const ClassGenerator::AttributeDesc &desc, const ClassGenerator::ClassDef &cls)
{
    if (desc.name == "phases") {
        return outputAttribute(cls, AttrDef("phases", ArkhorScriptParser::EnumValue, "ArkhamEncountery"), true);
    }
    return QuickOptionGenerator::outputDefaultAttribute(desc, cls);
}


}
