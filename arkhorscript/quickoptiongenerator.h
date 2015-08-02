#ifndef QUICKOPTIONGENERATOR_H
#define QUICKOPTIONGENERATOR_H

#include "optiongenerator.h"

namespace AHS {

class QuickOptionGenerator : public OptionGenerator
{
public:
    QuickOptionGenerator(QTextStream &out) : OptionGenerator(out) {}
    virtual bool generate(const ClassDef &cls);

protected:
    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
};

}

#endif // QUICKOPTIONGENERATOR_H
