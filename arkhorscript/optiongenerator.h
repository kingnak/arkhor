#ifndef OPTIONGENERATOR_H
#define OPTIONGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class OptionGenerator : public ClassGenerator
{
public:
    OptionGenerator(QTextStream &out) : ClassGenerator(out) {}

    virtual bool generate(const ClassDef &cls);

protected:
    void outputCreateOptionStart(const ClassDef &cls);
    void outputRegisterOption(const ClassDef &cls);

    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
    //virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);
};

}

#endif // OPTIONGENERATOR_H
