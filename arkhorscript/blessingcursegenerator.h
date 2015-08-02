#ifndef BLESSINGCURSEGENERATOR_H
#define BLESSINGCURSEGENERATOR_H

#include "objectgenerator.h"

namespace AHS {

class BlessingCurseGenerator : public ObjectGenerator
{
public:
    BlessingCurseGenerator(QTextStream &out) : ObjectGenerator(out) {}
    bool generate(const ClassGenerator::ClassDef &cls);

};

}

#endif // BLESSINGCURSEGENERATOR_H
