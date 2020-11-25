#ifndef ANCIENTONEGENERATOR_H
#define ANCIENTONEGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class AncientOneGenerator : public ClassGenerator
{
public:
    AncientOneGenerator(QTextStream &out) : ClassGenerator(out) {}
    bool generate(const ClassDef &cls) override;

protected:
    void outputCreateAncientOneStart(const ClassDef &cls);
    void outputRegisterAncientOne(const ClassDef &cls);

    QList<AttributeDesc> getAttributes() override;
    bool outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr) override;
    //virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);

};

}

#endif // ANCIENTONEGENERATOR_H
