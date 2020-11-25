#ifndef SPELLGENERATOR_H
#define SPELLGENERATOR_H

#include "objectgenerator.h"

namespace AHS {

class SpellGenerator : public ObjectGenerator
{
public:
    SpellGenerator(QTextStream &out) : ObjectGenerator(out) {}

protected:
    QList<AttributeDesc> getAttributes() override;
    bool outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr) override;
    bool outputDefaultAttribute(const AttributeDesc &desc, const ClassGenerator::ClassDef &cls) override;
};

}

#endif // SPELLGENERATOR_H
