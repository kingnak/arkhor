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


}
