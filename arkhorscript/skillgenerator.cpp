#include "skillgenerator.h"


QList<AHS::ClassGenerator::AttributeDesc> AHS::SkillGenerator::getAttributes()
{
    return ObjectGenerator::getAttributes()
            << AttributeDesc("exhaustable", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
               ;
}
