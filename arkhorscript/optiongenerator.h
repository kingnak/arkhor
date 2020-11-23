#ifndef OPTIONGENERATOR_H
#define OPTIONGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class OptionGenerator : public ClassGenerator
{
public:
    OptionGenerator(QTextStream &out) : ClassGenerator(out) {}

    bool generate(const ClassDef &cls) override;

protected:
    //virtual bool allowNested(const ClassDef &cls) const { Q_UNUSED(cls); return true; }

    void outputCreateOptionStart(const ClassDef &cls);
    void outputRegisterOption(const ClassDef &cls);

    QList<AttributeDesc> getAttributes() override;
    bool outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr) override;
    //virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);
};

}

#endif // OPTIONGENERATOR_H
