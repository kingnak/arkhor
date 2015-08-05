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
               ;
}

bool AncientOneGenerator::outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr)
{
    if (desc.name == "defenses") {
        return outputEnumValue("Constants.Monster", attr, cls);
    }
    if (desc.name == "slumberModifications") {
        return outputModifications(attr, cls);
    }
    if (desc.name == "monsterModifications") {
        if (attr.type == AttrDef::Literal) {
            m_out << attr.content;
            return true;
        } else if (attr.type == AttrDef::Complex) {
            return outputMonsterModifications(attr.content, cls);
        } else {
            return setError("monsterModifications must be Complex or Literal", cls);
        }
    }

    Q_ASSERT_X(false, "AncientOne Generator", qPrintable(QString("Special Attribute '%1' not handled").arg(desc.name)));
    return false;
}


}
