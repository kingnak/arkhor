#include "blessingcursegenerator.h"

namespace AHS
{

bool BlessingCurseGenerator::generate(const AHS::ClassGenerator::ClassDef &cls)
{
    ClassGenerator::ClassDef copy(cls);
    if (!copy.hasElemMult) copy.elemMult = -1;
    outputClassComment(copy);
    outputCreateObjectStart(copy);
    if (!outputAttributes(copy)) return false;
    outputCreateEnd(copy);
    outputRegisterConstant(copy);
    outputRegisterMultiObject(copy);
    return true;

}

}
