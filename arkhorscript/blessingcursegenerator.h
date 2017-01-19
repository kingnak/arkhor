#ifndef BLESSINGCURSEGENERATOR_H
#define BLESSINGCURSEGENERATOR_H

#include "objectgenerator.h"

namespace AHS {

class BlessingCurseGenerator : public ObjectGenerator
{
public:
    BlessingCurseGenerator(QTextStream &out) : ObjectGenerator(out) {}
    bool generate(const ClassGenerator::ClassDef &cls);

    bool allowInfinite(const ClassDef &cls) const { Q_UNUSED(cls); return true; }
};

}

#endif // BLESSINGCURSEGENERATOR_H
