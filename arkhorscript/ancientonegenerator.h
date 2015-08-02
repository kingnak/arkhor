#ifndef ANCIENTONEGENERATOR_H
#define ANCIENTONEGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class AncientOneGenerator : public ClassGenerator
{
public:
    AncientOneGenerator(QTextStream &out) : ClassGenerator(out) {}
    virtual bool generate(const ClassDef &cls);

protected:
    void outputCreateAncientOneStart(const ClassDef &cls);
    void outputRegisterAncientOne(const ClassDef &cls);

    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
    //virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);

private:
    bool outputMonsterModifications(QString v, const ClassDef &cls);
};

}

#endif // ANCIENTONEGENERATOR_H
