#ifndef ARKHAMENCOUNTERGENERATOR_H
#define ARKHAMENCOUNTERGENERATOR_H

#include "classgenerator.h"

namespace AHS {

class ArkhamEncounterGenerator : public ClassGenerator
{
public:
    ArkhamEncounterGenerator(QTextStream &out) : ClassGenerator(out) {}

    virtual bool generate(const ClassDef &cls);

protected:
    void outputCreateArkhamEncounterStart(const ClassDef &cls);
    void outputRegisterAkhamEncounter(const ClassDef &cls);

    virtual QList<AttributeDesc> getAttributes();
    virtual bool outputSpecialAttribute(AttributeDesc desc, const ClassDef &cls, const AttrDef &attr);
    //virtual bool outputDefaultAttribute(AttributeDesc desc, const ClassDef &cls);

};

}

#endif // ARKHAMENCOUNTERGENERATOR_H
