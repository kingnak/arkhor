#include "specialabilitygenerator.h"

namespace AHS {

bool SpecialAbilityGenerator::generate(const ClassDef &cls)
{
    outputClassComment(cls);
    outputCreateObjectStart(cls);
    if (!outputAttributes(cls)) return false;
    outputCreateEnd(cls);
    outputRegisterConstant(cls);
    outputRegisterSingleObject(cls);
    return true;
}

}
