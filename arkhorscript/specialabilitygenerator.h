#ifndef SPECIALABILITYGENERATOR_H
#define SPECIALABILITYGENERATOR_H

#include "specialobjectgenerator.h"

namespace AHS {

class SpecialAbilityGenerator : public SpecialObjectGenerator
{
public:
    SpecialAbilityGenerator(QTextStream &out) : SpecialObjectGenerator(out) {}

    bool generate(const ClassDef &cls);
};

}

#endif // SPECIALABILITYGENERATOR_H
