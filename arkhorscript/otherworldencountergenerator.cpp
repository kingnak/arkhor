#include "otherworldencountergenerator.h"

namespace AHS
{

bool OtherWorldEncounterGenerator::generate(const ClassDef &cls)
{
    outputClassComment(cls);
    outputCreateOtherWorldEncounterStart(cls);
    if (!outputAttributes(cls)) return false;
    outputCreateEnd(cls);
    outputRegisterConstant(cls);
    outputRegisterOtherWorldEncounter(cls);
    return true;
}

void OtherWorldEncounterGenerator::outputCreateOtherWorldEncounterStart(const ClassDef &cls)
{
    outputCreateStart("OtherWorldEncounter", cls);
}

void OtherWorldEncounterGenerator::outputRegisterOtherWorldEncounter(const ClassDef &cls)
{
    outputRegisterSingle("OtherWorldEncounter", cls);
}

QList<ClassGenerator::AttributeDesc> OtherWorldEncounterGenerator::getAttributes()
{
    return QList<AttributeDesc>()
            << AttributeDesc("id", AttributeDesc::R_Predefined, AttributeDesc::H_ID, AttributeDesc::V_Primitive)
            << AttributeDesc("description", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("field", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("color", AttributeDesc::R_Required, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("options", AttributeDesc::R_Required, AttributeDesc::H_IDRef, AttributeDesc::V_Array)
               ;
}

bool OtherWorldEncounterGenerator::outputSpecialAttribute(const ClassGenerator::AttributeDesc &desc, const ClassGenerator::ClassDef &cls, const ClassGenerator::AttrDef &attr)
{
    if (desc.name == "field") {
        return outputEnumValue("Constants.Fields", attr, cls);
    }
    if (desc.name == "color") {
        return outputEnumValue("Constants.OtherWorld", attr, cls);
    }
    Q_ASSERT_X(false, "OtherWorldEncounter Generator", qPrintable(QString("Special Attribute '%1' not handled").arg(desc.name)));
    return false;
}

}
