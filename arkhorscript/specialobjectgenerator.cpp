#include "specialobjectgenerator.h"

namespace AHS {

QList<ClassGenerator::AttributeDesc> SpecialObjectGenerator::getAttributes()
{
    return ObjectGenerator::getAttributes()
            << AttributeDesc("exhaustable", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("actions", AttributeDesc::R_Optional, AttributeDesc::H_IDRef, AttributeDesc::V_Array)
            << AttributeDesc("hands", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
            << AttributeDesc("attributes", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Array);
               ;
}

QString SpecialObjectGenerator::getObjectTypeName(const ClassGenerator::ClassDef &cls) const
{
    Q_UNUSED(cls);
    return "Special";
}

/*
bool SpecialObjectGenerator::generate(const ClassGenerator::ClassDef &cls)
{
    outputClassComment(cls);
    outputCreateObjectStart(cls);
    if (!outputAttributes(cls)) return false;
    outputCreateEnd(cls);
    outputRegisterMultiObject(cls);
    return true;
}
*/


}
