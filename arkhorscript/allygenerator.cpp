#include "allygenerator.h"

namespace AHS {

bool AllyGenerator::generate(const ClassDef &cls)
{
    outputClassComment(cls);
    outputCreateObjectStart(cls);
    if (!outputAttributes(cls)) return false;
    outputCreateEnd(cls);
    outputRegisterSingleObject(cls);
    return true;
}

}
