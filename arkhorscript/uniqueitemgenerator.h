#ifndef UNIQUEITEMGENERATOR_H
#define UNIQUEITEMGENERATOR_H

#include "objectgenerator.h"

namespace AHS {

class UniqueItemGenerator : public ObjectGenerator
{
public:
    UniqueItemGenerator(QTextStream &out) : ObjectGenerator(out) {}

protected:
    QList<AttributeDesc> getAttributes() override;
};

}

#endif // UNIQUEITEMGENERATOR_H
