#include "blessingcursegenerator.h"

namespace AHS
{

bool BlessingCurseGenerator::generate(const AHS::ClassGenerator::ClassDef &cls)
{
    outputClassComment(cls);
    outputCreateObjectStart(cls);
    if (!outputAttributes(cls)) return false;
    outputCreateEnd(cls);
    outputRegisterMultiObject(cls);
    return true;

}

}
