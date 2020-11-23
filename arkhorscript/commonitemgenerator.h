#ifndef COMMONITEMGENERATOR_H
#define COMMONITEMGENERATOR_H

#include "objectgenerator.h"

namespace AHS {

class CommonItemGenerator : public ObjectGenerator
{
public:
    CommonItemGenerator(QTextStream &out) : ObjectGenerator(out) {}

protected:
    QList<AttributeDesc> getAttributes() override;
};

}

#endif // COMMONITEMGENERATOR_H
