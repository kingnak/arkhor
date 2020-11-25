#ifndef SPECIALABILITYGENERATOR_H
#define SPECIALABILITYGENERATOR_H

#include "specialobjectgenerator.h"

namespace AHS {

class SpecialAbilityGenerator : public SpecialObjectGenerator
{
public:
    SpecialAbilityGenerator(QTextStream &out) : SpecialObjectGenerator(out) {}

    bool generate(const ClassDef &cls) override;

protected:
    //virtual bool allowNested(const ClassDef &cls) const { Q_UNUSED(cls); return true; }

};

}

#endif // SPECIALABILITYGENERATOR_H
