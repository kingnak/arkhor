#ifndef DIETESTHELPER_H
#define DIETESTHELPER_H

#include "ahglobal.h"
#include "propertymodifier.h"
#include <diedata.h>

class Character;
class DieRollBoolEvaluator;
class DieRollEvaluator;

class DieTestHelper
{
public:
    struct DieTestSpec {
        DieRollEvaluator *eval;
        AH::Common::ModifiedPropertyValueData baseVal;
        AH::Common::DieRollTestData data;
        DieTestSpec() : eval(0) {}
    };

    static DieTestSpec createSkillTest(Character *c, AH::Skill skill, int adjustment, int target = 1);
    static DieTestSpec createAttributeTest(Character *c, AH::Attribute attr, int adjustment, int target = 1);
    static DieTestSpec createGenericTest(int dieCount, QList<int> successVals, int target = 1);

    static DieTestSpec createGenericCounter(int dieCount, QList<int> successVals);
    static DieTestSpec createGenericSummer(int dieCount);

    static AH::Common::DieRollTestData toDieRollTestData(const DieTestSpec &spec);

private:
    DieTestHelper();
    Q_DISABLE_COPY(DieTestHelper)
};

#endif // DIETESTHELPER_H
