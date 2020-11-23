#ifndef INVESTIGATORGENERATOR_H
#define INVESTIGATORGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class InvestigatorGenerator : public ClassGenerator
{
public:
    InvestigatorGenerator(QTextStream &out) : ClassGenerator(out) {}

    bool generate(const ClassDef &cls) override;

protected:
    void outputCreateInvestigatorStart(const ClassDef &cls);
    void outputRegisterInvestigator(const ClassDef &cls);

    QList<AttributeDesc> getAttributes() override;
    bool outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr) override;
    //virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);

private:
    bool outputRandomPossesions(const QString &v);
    bool outputInvAttributes(QString v, const ClassDef &cls);
};

}

#endif // INVESTIGATORGENERATOR_H
