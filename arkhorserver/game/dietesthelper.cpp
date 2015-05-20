#include "dietesthelper.h"
#include "game.h"
#include "player.h"
#include "character.h"
#include "gamecontext.h"
#include "gameoption.h"
#include "die/diepool.h"
#include "die/dierollcountevaluator.h"
#include "die/dierollsumevaluator.h"
#include <QSet>

using namespace AH::Common;
/*
DieTestHelper::DieTestSpec DieTestHelper::createSkillTest(QString desc, Character *c, AH::Skill skill, int adjustment, int target)
{
    ModifiedPropertyValue poolBase = gGame->context().getCharacterSkill(c, skill);
    ModifiedPropertyValue clueBurnMods = gGame->context().getCharacterClueBurn(c, skill);
    return createClueBurnTest(desc, c, poolBase, clueBurnMods, adjustment, target);
}

DieTestHelper::DieTestSpec DieTestHelper::createSkillCounter(QString desc, Character *c, AH::Skill skill, int adjustment)
{
    ModifiedPropertyValue poolBase = gGame->context().getCharacterSkill(c, skill);
    // Get success values
    QSet<quint32> successes;
    for ( int i = gGame->context().getCharacterProperty(c, PropertyValue::Prop_MinSuccessDieRoll).finalVal(); i <= 6; ++i) {
        successes << i;
    }
}

DieTestHelper::DieTestSpec DieTestHelper::createGenericCounter(QString desc, int dieCount, QList<quint32> successVals)
{
    DieTestSpec spec;
    DiePool p = DiePool::createDiePool(QList<StandardDieSpec>() << StandardDieSpec(DieFactory::D6, dieCount));

    DieRollEvaluator *eval = new DieRollCountEvaluator(p, successVals.toSet());
    spec.eval = eval;

    AH::Common::DiePoolData poolData(dieCount, 0);

    AH::Common::DieRollData rollData(AH::Common::DieRollData::Count);
    rollData.setSuccessRolls(successVals);
    rollData.setPool(poolData);

    AH::Common::DieRollTestData testData(AH::Common::DieRollTestData::Value);
    testData.setRollData(rollData);
    testData.setDescription(desc);

    spec.data = testData;
    return spec;
}

DieTestHelper::DieTestSpec DieTestHelper::createGenericSummer(QString desc, int dieCount)
{
    DieTestSpec spec;
    DiePool p = DiePool::createDiePool(QList<StandardDieSpec>() << StandardDieSpec(DieFactory::D6, dieCount));
    DieRollEvaluator *eval = new DieRollSumEvaluator(p);
    spec.eval = eval;

    AH::Common::DiePoolData poolData(dieCount, 0);

    AH::Common::DieRollData rollData(AH::Common::DieRollData::Sum);
    rollData.setPool(poolData);

    AH::Common::DieRollTestData testData(AH::Common::DieRollTestData::Value);
    testData.setRollData(rollData);
    testData.setDescription(desc);

    spec.data = testData;
    return spec;
}

DieTestHelper::DieTestSpec DieTestHelper::createClueBurnTest(QString desc, Character *c, ModifiedPropertyValue poolBase, ModifiedPropertyValue clueBurnMods, int adjustment, int target)
{
    DieTestSpec spec;
    // calculate pool size
    spec.baseVal = poolBase.toModifiedPropertyValueData();
    int ct = poolBase.finalVal();
    ct += adjustment;

    // Get success values
    QSet<quint32> successes;
    for ( int i = gGame->context().getCharacterProperty(c, PropertyValue::Prop_MinSuccessDieRoll).finalVal(); i <= 6; ++i) {
        successes << i;
    }

    int dieCount = qMax(ct, 0);

    // Create pool and evaluator
    DiePool p = DiePool::createDiePool(QList<StandardDieSpec>() << StandardDieSpec(DieFactory::D6, dieCount));
    DieRollBoolEvaluator *ret = new DieRollCountBoolEvaluator(p, successes, target, DieRollBoolEvaluator::GREATER_EQUALS);
    spec.eval = ret;

    AH::Common::DiePoolData poolData(poolBase.toModifiedPropertyValueData(), adjustment);

    AH::Common::DieRollData rollData(AH::Common::DieRollData::Count);
    rollData.setPool(poolData);
    rollData.setSuccessRolls(successes.toList());

    AH::Common::DieRollTestData test(AH::Common::DieRollTestData::Boolean, target);
    test.setRollData(rollData);
    test.setDiceForClueBurn(clueBurnMods.finalVal());
    test.setClueBurnMods(clueBurnMods.toModifiedPropertyValueData());
    test.setDescription(desc);

    // Set die roll options
    spec.options = c->getOptions(AH::DieRoll);
    QStringList optIds;
    foreach (GameOption *opt, spec.options) {
        optIds << opt->id();
    }


    spec.data = test;

    return spec;
}
*/
DieTestHelper::DieTestSpec DieTestHelper::createClueBurnTest(QString desc, Character *c, ModifiedPropertyValue poolBase, ModifiedPropertyValue clueBurnMods, int adjustment, int target)
{
    DieTestSpec spec;
    propertyPool(spec, poolBase, adjustment);
    clueBurnProperty(spec, clueBurnMods);
    successCounter(spec, c);
    test(spec, target);
    finalize(spec);
    spec.data.setDescription(desc);
    return spec;
}

DieTestHelper::DieTestSpec DieTestHelper::createSkillTest(QString desc, Character *c, AH::Skill skill, int adjustment, int target)
{
    DieTestSpec spec;
    skillPool(spec, c, skill, adjustment);
    clueBurnSkill(spec, c, skill);
    successCounter(spec, c);
    test(spec, target);
    finalize(spec);
    spec.data.setDescription(desc);
    return spec;
}

DieTestHelper::DieTestSpec DieTestHelper::createSkillCounter(QString desc, Character *c, AH::Skill skill, int adjustment)
{
    DieTestSpec spec;
    skillPool(spec, c, skill, adjustment);
    successCounter(spec, c);
    value(spec);
    finalize(spec);
    spec.data.setDescription(desc);
    return spec;
}

DieTestHelper::DieTestResult DieTestHelper::executeDieTest(Player *p, DieTestHelper::DieTestSpec &spec)
{
    AH::Common::DieTestUpdateData upd;

    upd = p->dieRollStart(spec.data);
    bool cont;
    do {
        int ct = upd.clueBurnAmount();
        if (ct > 0) {
            // BURN CLUES!
            AH::Common::CostList lst;
            lst << AH::Common::CostItem(AH::Common::CostItem::Pay_Clue, ct);
            if (p->getCharacter()->canPay(lst)) {
                p->getCharacter()->pay(lst);

                int dieAdds = ct*spec.data.diceForClueBurn();

                AH::Common::DiePoolData &pool = spec.data.rollData().pool();

                int oldDieCount = pool.dieCount();
                pool.setDieCount(pool.dieCount() + dieAdds);

                // consider adjustment for actual die update!
                oldDieCount += pool.adjustment();

                // Special Case: when there was a negative count, don't simply add!
                int diceToAdd = dieAdds;
                if (oldDieCount < 0) diceToAdd += oldDieCount; // oldDieCount is negative!
                spec.eval->addDice(QList<StandardDieSpec>() << StandardDieSpec(DieFactory::D6, diceToAdd));
                gGame->commitUpdates();
            }
        }

        // TODO: Handle Reroll options!

        spec.eval->rollNew();
        spec.eval->evaluate();
        QList<quint32> dieVals;
        foreach (DieRollResultItem itm, spec.eval->pool()->getResult()) {
            dieVals << itm.value();
        }
        spec.data.rollData().pool().setDieValues(dieVals);

        if (dynamic_cast<const DieRollBoolEvaluator*>(spec.eval)) {
            spec.data.setSucceeded(dynamic_cast<const DieRollBoolEvaluator*>(spec.eval)->getBoolResult());
        }

        upd = p->dieRollUpdate(spec.data);
        cont = upd.clueBurnAmount() > 0 || !upd.dieRollOptionId().isEmpty();
    } while (cont);

    //p->dieRollFinish(spec.data);

    DieTestResult ret;
    ret.intResult = spec.eval->getResult();
    if (dynamic_cast<const DieRollBoolEvaluator*>(spec.eval)) {
        ret.boolResult = dynamic_cast<const DieRollBoolEvaluator*>(spec.eval)->getBoolResult();
    } else {
        ret.boolResult = false;
    }
    return ret;
}

void DieTestHelper::fixedPool(DieTestHelper::DieTestSpec &spec, int size)
{
    DiePoolData pd(size, 0);
    spec.data.rollData().setPool(pd);
}

void DieTestHelper::skillPool(DieTestHelper::DieTestSpec &spec, Character *c, AH::Skill skill, int adjust)
{
    ModifiedPropertyValue poolBase = gGame->context().getCharacterSkill(c, skill);
    spec.baseVal = poolBase.toModifiedPropertyValueData();
    DiePoolData pd(poolBase.toModifiedPropertyValueData(), adjust);
    spec.data.rollData().setPool(pd);
}

void DieTestHelper::propertyPool(DieTestHelper::DieTestSpec &spec, ModifiedPropertyValue poolBase, int adjust)
{
    spec.baseVal = poolBase.toModifiedPropertyValueData();
    DiePoolData pd(poolBase.toModifiedPropertyValueData(), adjust);
    spec.data.rollData().setPool(pd);
}

void DieTestHelper::clueBurnSkill(DieTestHelper::DieTestSpec &spec, Character *c, AH::Skill skill)
{
    ModifiedPropertyValue clueBurnMods = gGame->context().getCharacterClueBurn(c, skill);
    spec.data.setClueBurnMods(clueBurnMods.toModifiedPropertyValueData());
    spec.data.setDiceForClueBurn(clueBurnMods.finalVal());
}

void DieTestHelper::clueBurnProperty(DieTestHelper::DieTestSpec &spec, ModifiedPropertyValue clueBurnMods)
{
    spec.data.setClueBurnMods(clueBurnMods.toModifiedPropertyValueData());
    spec.data.setDiceForClueBurn(clueBurnMods.finalVal());
}

void DieTestHelper::summer(DieTestHelper::DieTestSpec &spec)
{
    spec.data.rollData().setType(DieRollData::Sum);
}

void DieTestHelper::counter(DieTestHelper::DieTestSpec &spec, QList<quint32> successes)
{
    spec.data.rollData().setType(DieRollData::Count);
    spec.data.rollData().setSuccessRolls(successes);
}

void DieTestHelper::successCounter(DieTestHelper::DieTestSpec &spec, Character *c)
{
    spec.data.rollData().setType(DieRollData::Count);
    QList<quint32> successes;
    for ( int i = gGame->context().getCharacterProperty(c, PropertyValue::Prop_MinSuccessDieRoll).finalVal(); i <= 6; ++i) {
        successes << i;
    }
    spec.data.rollData().setSuccessRolls(successes);
}

void DieTestHelper::value(DieTestHelper::DieTestSpec &spec)
{
    spec.data.setType(DieRollTestData::Value);
}

void DieTestHelper::test(DieTestHelper::DieTestSpec &spec, int target)
{
    spec.data.setType(DieRollTestData::Boolean);
    spec.data.setTargetValue(target);
}

void DieTestHelper::finalize(DieTestHelper::DieTestSpec &spec)
{
    int dieCt = spec.data.rollData().pool().dieCount() + spec.data.rollData().pool().adjustment();
    dieCt = qMax(0, dieCt);
    DiePool p = DiePool::createDiePool(QList<StandardDieSpec>() << StandardDieSpec(DieFactory::D6, dieCt));

    DieRollEvaluator *eval;
    switch (spec.data.rollData().type()) {
    case DieRollData::Sum:
        switch (spec.data.type()) {
        case DieRollTestData::Value:
            eval = new DieRollSumEvaluator(p);
            break;
            /*
        case DieRollTestData::Boolean:
            eval = new DieRollSumBoolEvaluator(p, spec.data.targetValue());
            break;
            */
        default:
            Q_ASSERT_X(false, "DieTestHelper::finalize", "Invalid Test Type");
        }
        break;

    case DieRollData::Count:
        switch (spec.data.type()) {
        case DieRollTestData::Value:
            eval = new DieRollCountEvaluator(p, spec.data.rollData().successRolls().toSet());
            break;
        case DieRollTestData::Boolean:
            eval = new DieRollCountBoolEvaluator(p, spec.data.rollData().successRolls().toSet(), spec.data.targetValue(), DieRollBoolEvaluator::GREATER_EQUALS);
            break;
        default:
            Q_ASSERT_X(false, "DieTestHelper::finalize", "Invalid Test Type");
        }
        break;
    default:
        Q_ASSERT_X(false, "DieTestHelper::finalize", "Invalid Test Type");
    }

    spec.eval = eval;
}
