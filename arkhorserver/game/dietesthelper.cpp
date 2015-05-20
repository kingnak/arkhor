#include "dietesthelper.h"
#include "game.h"
#include "character.h"
#include "gamecontext.h"
#include "die/diepool.h"
#include "die/dierollcountevaluator.h"
#include <QSet>

DieTestHelper::DieTestSpec DieTestHelper::createSkillTest(Character *c, AH::Skill skill, int adjustment, int target)
{
    DieTestSpec spec;
    // calculate pool size
    ModifiedPropertyValue v = gGame->context().getCharacterSkill(c, skill);
    spec.baseVal = v.toModifiedPropertyValueData();
    int ct = v.finalVal();
    ct += adjustment;

    // Get success values
    QSet<quint32> successes;
    for ( int i = gGame->context().getCharacterProperty(c, PropertyValue::Prop_MinSuccessDieRoll).finalVal(); i <= 6; ++i) {
        successes << i;
    }

    if (ct > 0) {
        // Create pool and evaluator
        DiePool p = DiePool::createDiePool(QList<StandardDieSpec>() << StandardDieSpec(DieFactory::D6, ct));
        DieRollBoolEvaluator *ret = new DieRollCountBoolEvaluator(p, successes, target, DieRollBoolEvaluator::GREATER_EQUALS);
        spec.eval = ret;
    }

    AH::Common::DiePoolData poolData(spec.baseVal, adjustment);
    AH::Common::DieRollData rollData(poolData, successes.toList());
    spec.data = AH::Common::DieRollTestData(rollData, target);

    return spec;
}

AH::Common::DieRollTestData DieTestHelper::toDieRollTestData(const DieTestHelper::DieTestSpec &spec)
{
    return spec.data;
}
