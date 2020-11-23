#include "arkhamencountergenerator.h"

namespace AHS
{

bool ArkhamEncounterGenerator::generate(const ClassDef &cls)
{
    outputClassComment(cls);
    outputCreateArkhamEncounterStart(cls);
    if (!outputAttributes(cls)) return false;
    outputCreateEnd(cls);
    outputRegisterConstant(cls);
    outputRegisterAkhamEncounter(cls);
    return true;
}

void ArkhamEncounterGenerator::outputCreateArkhamEncounterStart(const ClassDef &cls)
{
    outputCreateStart("ArkhamEncounter", cls);
}

void ArkhamEncounterGenerator::outputRegisterAkhamEncounter(const ClassDef &cls)
{
    outputRegisterSingle("ArkhamEncounter", cls);
}

QList<ClassGenerator::AttributeDesc> ArkhamEncounterGenerator::getAttributes()
{
    return QList<AttributeDesc>()
            << AttributeDesc("id", AttributeDesc::R_Predefined, AttributeDesc::H_ID, AttributeDesc::V_Primitive)
            << AttributeDesc("description", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("field", AttributeDesc::R_Required, AttributeDesc::H_Special, AttributeDesc::V_Primitive)
            << AttributeDesc("options", AttributeDesc::R_Required, AttributeDesc::H_IDRef, AttributeDesc::V_Array)
               ;
}

bool ArkhamEncounterGenerator::outputSpecialAttribute(const AttributeDesc &desc, const ClassGenerator::ClassDef &cls, const ClassGenerator::AttrDef &attr)
{
    if (desc.name == "field") {
        return outputEnumValue("Constants.Fields", attr, cls);
    }

    Q_ASSERT_X(false, "ArkhamEncounter Generator", qPrintable(QString("Special Attribute '%1' not handled").arg(desc.name)));
    return false;
}

}
