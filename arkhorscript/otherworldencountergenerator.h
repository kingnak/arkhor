#ifndef OTHERWORLDENCOUNTERGENERATOR_H
#define OTHERWORLDENCOUNTERGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class OtherWorldEncounterGenerator : public ClassGenerator
{
public:
    OtherWorldEncounterGenerator(QTextStream &out) : ClassGenerator(out) {}

    bool generate(const ClassDef &cls) override;

protected:
    bool allowAnonymous(const ClassDef &cls) const override { Q_UNUSED(cls); return true; }

    void outputCreateOtherWorldEncounterStart(const ClassDef &cls);
    void outputRegisterOtherWorldEncounter(const ClassDef &cls);

    QList<AttributeDesc> getAttributes() override;
    bool outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr) override;
    //virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);

};

}

#endif // OTHERWORLDENCOUNTERGENERATOR_H
