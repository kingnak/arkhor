#ifndef SKILLGENERATOR_H
#define SKILLGENERATOR_H

#include "objectgenerator.h"

namespace AHS {

class SkillGenerator : public ObjectGenerator
{
public:
    SkillGenerator(QTextStream &out) : ObjectGenerator(out) {}

    QList<AttributeDesc> getAttributes() override;
};

}

#endif // SKILLGENERATOR_H
