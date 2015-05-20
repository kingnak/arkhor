#include "dietesthelper.h"
#include "game.h"
#include "player.h"
#include "character.h"
#include "gamecontext.h"
#include "gameoption.h"
#include "die/diepool.h"
#include "die/dierollcountevaluator.h"
#include <QSet>

DieTestHelper::DieTestSpec DieTestHelper::createSkillTest(Character *c, AH::Skill skill, int adjustment, int target)
{
    ModifiedPropertyValue poolBase = gGame->context().getCharacterSkill(c, skill);
    ModifiedPropertyValue clueBurnMods = gGame->context().getCharacterClueBurn(c, skill);
    return createClueBurnTest(c, poolBase, clueBurnMods, adjustment, target);
}

DieTestHelper::DieTestSpec DieTestHelper::createClueBurnTest(Character *c, ModifiedPropertyValue poolBase, ModifiedPropertyValue clueBurnMods, int adjustment, int target)
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

    AH::Common::DiePoolData poolData(ct, adjustment);

    AH::Common::DieRollData rollData(AH::Common::DieRollData::Count);
    rollData.setPool(poolData);
    rollData.setSuccessRolls(successes.toList());

    AH::Common::DieRollTestData test(AH::Common::DieRollTestData::Boolean, target);
    test.setRollData(rollData);
    test.setDiceForClueBurn(clueBurnMods.finalVal());

    // Set die roll options
    spec.options = c->getOptions(AH::DieRoll);
    QStringList optIds;
    foreach (GameOption *opt, spec.options) {
        optIds << opt->id();
    }


    spec.data = test;

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

                // Special Case: when there was a negative count, don't simply add!
                int diceToAdd = dieAdds;
                if (oldDieCount < 0) diceToAdd += oldDieCount; // oldDieCount is negative!
                spec.eval->addDice(QList<StandardDieSpec>() << StandardDieSpec(DieFactory::D6, diceToAdd));
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
