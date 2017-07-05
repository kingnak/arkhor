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
        QString sourceId;
    };

    struct DieTestResult {
        bool boolResult;
        int intResult;
        DieTestResult() : boolResult(false), intResult(0) {}
    };

    static DieTestSpec createClueBurnTest(QString desc, QString sourceId, Character *c, ModifiedPropertyValue poolBase, ModifiedPropertyValue clueBurnMods, int adjustment, int target = 1);
    static DieTestSpec createClueBurnCounter(QString desc, QString sourceId, Character *c, ModifiedPropertyValue poolBase, ModifiedPropertyValue clueBurnMods, int adjustment);

    static DieTestSpec createSkillTest(QString desc, QString sourceId, Character *c, AH::Skill skill, int adjustment, int target = 1);
    //static DieTestSpec createAttributeTest(Character *c, AH::Attribute attr, int adjustment, int target = 1);
    //static DieTestSpec createGenericTest(int dieCount, QList<int> successVals, int target = 1);

    static DieTestSpec createSkillCounter(QString desc, QString sourceId, Character *c, AH::Skill skill, int adjustment);

    static DieTestSpec createGenericCounter(QString desc, QString sourceId, int dieCount, QList<quint32> successVals);
    static DieTestSpec createGenericSummer(QString desc, QString sourceId, int dieCount);

    static DieTestSpec &addDieRollOptions(DieTestSpec &spec, Character *c, AH::Skill skill);

    static DieTestResult executeDieTest(Player *p, DieTestSpec &spec);

private:
    static void fixedPool(DieTestSpec &spec, int size);
    static void skillPool(DieTestSpec &spec, Character *c, AH::Skill skill, int adjust);
    static void propertyPool(DieTestSpec &spec, ModifiedPropertyValue poolBase, int adjust);
    static void clueBurnSkill(DieTestSpec &spec, Character *c, AH::Skill skill);
    static void clueBurnProperty(DieTestSpec &spec, ModifiedPropertyValue clueBurnMods);
    static void summer(DieTestSpec &spec);
    static void counter(DieTestSpec &spec, QList<quint32> successes);
    static void successCounter(DieTestSpec &spec, Character *c);
    static void value(DieTestSpec &spec);
    static void test(DieTestSpec &spec, int target);
    static void finalize(DieTestSpec &spec, const QString &desc, const QString &sourceId);

    static void updateReRollOptions(DieTestSpec &spec);

private:
    DieTestHelper();
    Q_DISABLE_COPY(DieTestHelper)
};

#endif // DIETESTHELPER_H
