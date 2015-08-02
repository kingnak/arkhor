#ifndef SPECIALOBJECTGENERATOR_H
#define SPECIALOBJECTGENERATOR_H

#include "objectgenerator.h"

namespace AHS {

class SpecialObjectGenerator : public ObjectGenerator
{
public:
    SpecialObjectGenerator(QTextStream &out) : ObjectGenerator(out) {}
    //bool generate(const ClassGenerator::ClassDef &cls);

protected:
    //QList<AttributeDesc> getAttributes();

};

}

#endif // SPECIALOBJECTGENERATOR_H
