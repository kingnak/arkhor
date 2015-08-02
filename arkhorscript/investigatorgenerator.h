#ifndef INVESTIGATORGENERATOR_H
#define INVESTIGATORGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class InvestigatorGenerator : public ClassGenerator
{
public:
    InvestigatorGenerator(QTextStream &out) : ClassGenerator(out) {}

    virtual bool generate(const ClassDef &cls);

protected:
    void outputCreateInvestigatorStart(const ClassDef &cls);
    void outputRegisterInvestigator(const ClassDef &cls);

    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
    //virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);

private:
    bool outputRandomPossesions(QString v);
    bool outputInvAttributes(QString v, const ClassDef &cls);
};

}

#endif // INVESTIGATORGENERATOR_H
