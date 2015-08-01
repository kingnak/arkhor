#include "uniqueitemgenerator.h"

namespace AHS {

QList<ClassGenerator::AttributeDesc> UniqueItemGenerator::getAttributes()
{
    return ObjectGenerator::getAttributes()
        << AttributeDesc("hands", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
        << AttributeDesc("price", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
        << AttributeDesc("attributes", AttributeDesc::R_Optional, AttributeDesc::H_Special, AttributeDesc::V_Array);
}

}

