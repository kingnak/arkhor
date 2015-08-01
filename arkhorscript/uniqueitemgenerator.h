#ifndef UNIQUEITEMGENERATOR_H
#define UNIQUEITEMGENERATOR_H

#include "objectgenerator.h"

namespace AHS {

class UniqueItemGenerator : public ObjectGenerator
{
public:
    UniqueItemGenerator(QTextStream &out) : ObjectGenerator(out) {}

protected:
    virtual QList<AttributeDesc> getAttributes();
};

}

#endif // UNIQUEITEMGENERATOR_H
