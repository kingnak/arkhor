#ifndef OBJECTGENERATOR_H
#define OBJECTGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class ObjectGenerator : public ClassGenerator
{
public:
    ObjectGenerator(QTextStream &out);
    bool generate(const ClassDef &cls);

protected:
    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
    //virtual QString getType() const = 0;
};

}

#endif // OBJECTGENERATOR_H
