#ifndef ALLYGENERATOR_H
#define ALLYGENERATOR_H

#include "objectgenerator.h"

namespace AHS {

class AllyGenerator : public ObjectGenerator
{
public:
    AllyGenerator(QTextStream &out) : ObjectGenerator(out) {}
    bool generate(const ClassGenerator::ClassDef &cls);

protected:
    //QList<AttributeDesc> getAttributes();
};

}

#endif // ALLYGENERATOR_H
