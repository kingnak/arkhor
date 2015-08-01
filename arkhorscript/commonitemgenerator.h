#ifndef COMMONITEMGENERATOR_H
#define COMMONITEMGENERATOR_H

#include "objectgenerator.h"

namespace AHS {

class CommonItemGenerator : public ObjectGenerator
{
public:
    CommonItemGenerator(QTextStream &out) : ObjectGenerator(out) {}

protected:
    virtual QList<AttributeDesc> getAttributes();
};

}

#endif // COMMONITEMGENERATOR_H
