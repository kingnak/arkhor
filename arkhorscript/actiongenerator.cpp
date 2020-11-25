#include "actiongenerator.h"

namespace AHS
{

bool ActionGenerator::generate(const ClassGenerator::ClassDef &cls)
{
    outputClassComment(cls);
    outputCreateActionStart(cls);
    if (!outputAttributes(cls)) return false;
    outputCreateEnd(cls);
    outputRegisterConstant(cls);
    outputRegisterAction(cls);
    return true;
}

void ActionGenerator::outputCreateActionStart(const ClassGenerator::ClassDef &cls)
{
    outputCreateStart("Action", cls);
}

void ActionGenerator::outputRegisterAction(const ClassGenerator::ClassDef &cls)
{
    outputRegisterSingle("Action", cls);
}

QList<ClassGenerator::AttributeDesc> ActionGenerator::getAttributes()
{
    return QList<AttributeDesc>()
            << AttributeDesc("id", AttributeDesc::R_Predefined, AttributeDesc::H_ID, AttributeDesc::V_Primitive)
            << AttributeDesc("name", AttributeDesc::R_Default, AttributeDesc::H_Name, AttributeDesc::V_Primitive)
            << AttributeDesc("description", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("phases", AttributeDesc::R_Required, AttributeDesc::H_Special, AttributeDesc::V_Array | AttributeDesc::V_Primitive)
            << AttributeDesc("activate", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Function)
               ;
}

bool ActionGenerator::outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr)
{
    if (desc.name == "phases") {
        return outputEnumValueArray("Constants.GamePhases", attr, cls);
    }

    Q_ASSERT_X(false, "Action Generator", qPrintable(QString("Special Attribute '%1' not handled").arg(desc.name)));
    return false;
}


}
