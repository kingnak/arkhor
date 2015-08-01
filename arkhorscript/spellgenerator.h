#ifndef SPELLGENERATOR_H
#define SPELLGENERATOR_H

#include "objectgenerator.h"

namespace AHS {

class SpellGenerator : public ObjectGenerator
{
public:
    SpellGenerator(QTextStream &out) : ObjectGenerator(out) {}

protected:
    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
    virtual bool outputDefaultAttribute(ClassGenerator::AttributeDesc desc, const ClassGenerator::ClassDef &cls);
};

}

#endif // SPELLGENERATOR_H
