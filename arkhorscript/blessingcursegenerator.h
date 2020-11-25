#ifndef BLESSINGCURSEGENERATOR_H
#define BLESSINGCURSEGENERATOR_H

#include "objectgenerator.h"

namespace AHS {

class BlessingCurseGenerator : public ObjectGenerator
{
public:
    BlessingCurseGenerator(QTextStream &out) : ObjectGenerator(out) {}
    bool generate(const ClassGenerator::ClassDef &cls) override;

    bool allowInfinite(const ClassDef &cls) const override { Q_UNUSED(cls); return true; }
};

}

#endif // BLESSINGCURSEGENERATOR_H
