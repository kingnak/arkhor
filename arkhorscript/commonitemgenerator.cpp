#include "commonitemgenerator.h"

#include <QTextStream>

namespace AHS {

CommonItemGenerator::CommonItemGenerator(QTextStream &out)
    : ObjectGenerator(out)
{
}

QList<ClassGenerator::AttributeDesc> CommonItemGenerator::getAttributes()
{
    return ObjectGenerator::getAttributes()
        << AttributeDesc("hands", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive)
        << AttributeDesc("price", AttributeDesc::R_Optional, AttributeDesc::H_Simple, AttributeDesc::V_Primitive);
}

}
