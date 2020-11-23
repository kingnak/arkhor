#ifndef ARKHAMENCOUNTERGENERATOR_H
#define ARKHAMENCOUNTERGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class ArkhamEncounterGenerator : public ClassGenerator
{
public:
    ArkhamEncounterGenerator(QTextStream &out) : ClassGenerator(out) {}

    bool generate(const ClassDef &cls) override;

protected:
    bool allowAnonymous(const ClassDef &cls) const override { Q_UNUSED(cls); return true; }

    void outputCreateArkhamEncounterStart(const ClassDef &cls);
    void outputRegisterAkhamEncounter(const ClassDef &cls);

    QList<AttributeDesc> getAttributes() override;
    bool outputSpecialAttribute(const AttributeDesc &desc, const ClassDef &cls, const AttrDef &attr) override;
    //bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls) override;

};

}

#endif // ARKHAMENCOUNTERGENERATOR_H
