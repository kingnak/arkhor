#ifndef SKILLGENERATOR_H
#define SKILLGENERATOR_H

#include "objectgenerator.h"

namespace AHS {

class SkillGenerator : public ObjectGenerator
{
public:
    SkillGenerator(QTextStream &out) : ObjectGenerator(out) {}
};

}

#endif // SKILLGENERATOR_H
