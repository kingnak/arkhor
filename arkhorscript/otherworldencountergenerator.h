#ifndef OTHERWORLDENCOUNTERGENERATOR_H
#define OTHERWORLDENCOUNTERGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class OtherWorldEncounterGenerator : public ClassGenerator
{
public:
    OtherWorldEncounterGenerator(QTextStream &out) : ClassGenerator(out) {}

    virtual bool generate(const ClassDef &cls);

protected:
    bool allowAnonymous(const ClassDef &cls) const { Q_UNUSED(cls); return true; }

    void outputCreateOtherWorldEncounterStart(const ClassDef &cls);
    void outputRegisterOtherWorldEncounter(const ClassDef &cls);

    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
    //virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);

};

}

#endif // OTHERWORLDENCOUNTERGENERATOR_H
