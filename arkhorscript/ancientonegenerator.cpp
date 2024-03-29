#include "ancientonegenerator.h"

#include <QTextStream>

namespace AHS
{

bool AncientOneGenerator::generate(const ClassGenerator::ClassDef &cls)
{
    outputClassComment(cls);
    outputCreateAncientOneStart(cls);
    if (!outputAttributes(cls)) return false;
    outputCreateEnd(cls);
    outputRegisterConstant(cls);
    outputRegisterAncientOne(cls);
    return true;
}

void AncientOneGenerator::outputCreateAncientOneStart(const ClassGenerator::ClassDef &cls)
{
    outputCreateStart("AncientOne", cls);
}

void AncientOneGenerator::outputRegisterAncientOne(const ClassGenerator::ClassDef &cls)
{
    outputRegisterSingle("AncientOne", cls);
}

QList<ClassGenerator::AttributeDesc> AncientOneGenerator::getAttributes()
{
    return QList<AttributeDesc>()
            << AttributeDesc("id", AttributeDesc::R_Predefined, AttributeDesc::H_ID, AttributeDesc::V_Primitive)
            << AttributeDesc("name", AttributeDesc::R_Default, AttributeDesc::H_Name, AttributeDesc::V_Primitive)
            << AttributeDesc("combatAdjustment", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("worshippersText", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("powerTitle", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("powerText", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("battleStartText", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("attackText", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("defenses", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Array)
            << AttributeDesc("doomTrack", AttributeDesc::R_Required, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("onAwake", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("attackAdjustment", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("attack", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("slumberModifications", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Array)
            << AttributeDesc("monsterModifications", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Complex)
            << AttributeDesc("monsterMoveModifications", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Complex)
            << AttributeDesc("postAttack", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("onEndMythos", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Function)
            << AttributeDesc("ignoreEnvironmentTypes", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Array | AttributeDesc::V_Primitive)
            << AttributeDesc("onUnconscious", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
            << AttributeDesc("onInsane", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
            << AttributeDesc("onLostInSpaceAndTime", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
            << AttributeDesc("onDefeatMonster", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Function)
               ;
}

bool AncientOneGenerator::outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr)
{
    if (desc.name == "defenses") {
        return outputEnumValue("Constants.Monster", attr, cls);
    }
    if (desc.name == "slumberModifications") {
        return outputModifications(attr, cls);
    }
    if (desc.name == "monsterModifications") {
        return outputMonsterModifications(attr, cls);
    }
    if (desc.name == "monsterMoveModifications") {
        return outputMonsterMoveModifications(attr, cls);
    }
    if (desc.name == "ignoreEnvironmentTypes") {
        return outputEnumValueArray("Constants.EnvironmentType", attr, cls);
    }
    if (desc.name == "onUnconscious" || desc.name == "onInsane" || desc.name == "onLostInSpaceAndTime") {
        return outputFunction(attr, cls, "c");
    }
    if (desc.name == "onDefeatMonster") {
        return outputFunction(attr, cls, "c, m");
    }
    Q_ASSERT_X(false, "AncientOne Generator", qPrintable(QString("Special Attribute '%1' not handled").arg(desc.name)));
    return false;
}


}
