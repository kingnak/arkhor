#ifndef DIETESTHELPER_H
#define DIETESTHELPER_H

#include "ahglobal.h"
#include "propertymodifier.h"
#include <diedata.h>

class Character;
class Player;
class GameOption;
class DieRollBoolEvaluator;
class DieRollEvaluator;

class DieTestHelper
{
public:
    struct DieTestSpec {
        DieRollEvaluator *eval;
        AH::Common::ModifiedPropertyValueData baseVal;
        AH::Common::DieRollTestData data;
        QList<GameOption *> options;
        DieTestSpec() : eval(0) {}
    };

    struct DieTestResult {
        bool boolResult;
        int intResult;
        DieTestResult() : boolResult(false), intResult(0) {}
    };

    static DieTestSpec createClueBurnTest(QString desc, Character *c, ModifiedPropertyValue poolBase, ModifiedPropertyValue clueBurnMods, int adjustment, int target = 1);

    static DieTestSpec createSkillTest(QString desc, Character *c, AH::Skill skill, int adjustment, int target = 1);
    //static DieTestSpec createAttributeTest(Character *c, AH::Attribute attr, int adjustment, int target = 1);
    //static DieTestSpec createGenericTest(int dieCount, QList<int> successVals, int target = 1);

    //static DieTestSpec createGenericCounter(int dieCount, QList<int> successVals);
    //static DieTestSpec createGenericSummer(int dieCount);

    static DieTestResult executeDieTest(Player *p, DieTestSpec &spec);

private:
    DieTestHelper();
    Q_DISABLE_COPY(DieTestHelper)
};

#endif // DIETESTHELPER_H
