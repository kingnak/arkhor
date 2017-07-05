#include "dietesthelper.h"
#include "game.h"
#include "player.h"
#include "character.h"
#include "gamecontext.h"
#include "gameoption.h"
#include "die/diepool.h"
#include "die/dierollcountevaluator.h"
#include "die/dierollsumevaluator.h"
#include "gameoption.h"
#include "actions/dierollaction.h"
#include <QSet>

using namespace AH::Common;

DieTestHelper::DieTestSpec DieTestHelper::createClueBurnTest(QString desc, QString sourceId, Character *c, ModifiedPropertyValue poolBase, ModifiedPropertyValue clueBurnMods, int adjustment, int target)
{
    DieTestSpec spec;
    propertyPool(spec, poolBase, adjustment);
    clueBurnProperty(spec, clueBurnMods);
    successCounter(spec, c);
    test(spec, target);
    finalize(spec, desc, sourceId);
    return spec;
}

DieTestHelper::DieTestSpec DieTestHelper::createClueBurnCounter(QString desc, QString sourceId, Character *c, ModifiedPropertyValue poolBase, ModifiedPropertyValue clueBurnMods, int adjustment)
{
    DieTestSpec spec;
    propertyPool(spec, poolBase, adjustment);
    clueBurnProperty(spec, clueBurnMods);
    successCounter(spec, c);
    value(spec);
    finalize(spec, desc, sourceId);
    return spec;
}

DieTestHelper::DieTestSpec DieTestHelper::createSkillTest(QString desc, QString sourceId, Character *c, AH::Skill skill, int adjustment, int target)
{
    DieTestSpec spec;
    skillPool(spec, c, skill, adjustment);
    clueBurnSkill(spec, c, skill);
    successCounter(spec, c);
    test(spec, target);
    finalize(spec, desc, sourceId);
    return spec;
}

DieTestHelper::DieTestSpec DieTestHelper::createSkillCounter(QString desc, QString sourceId, Character *c, AH::Skill skill, int adjustment)
{
    DieTestSpec spec;
    skillPool(spec, c, skill, adjustment);
    clueBurnSkill(spec, c, skill);
    successCounter(spec, c);
    value(spec);
    finalize(spec, desc, sourceId);
    return spec;
}

DieTestHelper::DieTestSpec DieTestHelper::createGenericCounter(QString desc, QString sourceId, int dieCount, QList<quint32> successVals)
{
    DieTestSpec spec;
    fixedPool(spec, dieCount);
    counter(spec, successVals);
    value(spec);
    finalize(spec, desc, sourceId);
    return spec;
}

DieTestHelper::DieTestSpec DieTestHelper::createGenericSummer(QString desc, QString sourceId, int dieCount)
{
    DieTestSpec spec;
    fixedPool(spec, dieCount);
    summer(spec);
    value(spec);
    finalize(spec, desc, sourceId);
    return spec;
}

DieTestHelper::DieTestSpec &DieTestHelper::addDieRollOptions(DieTestHelper::DieTestSpec &spec, Character *c, AH::Skill skill)
{
    spec.data.rollData().setSkill(skill);
    for (auto obj : c->inventory()) {
        for (auto o : obj->getOptions()) {
            if (auto dro = dynamic_cast<DieRollOption*> (o)) {
                if (dro->skills().testFlag(skill)) {
                    spec.options << o;
                    dro->reset();
                    dro->setSource(obj);
                }
            }
        }
    }

    return spec;
}

DieTestHelper::DieTestResult DieTestHelper::executeDieTest(Player *p, DieTestHelper::DieTestSpec &spec)
{
    AH::Common::DieTestUpdateData upd;

    gGame->context().setDieRoll(&spec);

    updateReRollOptions(spec);

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

        // Handle Reroll options!
        if (!upd.dieRollOptionId().isEmpty()) {
            GameOption *op = NULL;
            foreach (GameOption *o, spec.options) {
                if (o->id() == upd.dieRollOptionId()) {
                    op = o;
                    break;
                }
            }

            DieRollOption *dro = dynamic_cast<DieRollOption *> (op);
            if (dro) {
                dro->execute();
            }
        }

        spec.eval->rollNew();
        QList<quint32> dieVals;
        foreach (DieRollResultItem itm, spec.eval->pool()->getResult()) {
            dieVals << itm.value();
        }
        spec.data.rollData().pool().setDieValues(dieVals);

        if (dynamic_cast<const DieRollBoolEvaluator*>(spec.eval)) {
            spec.data.setSucceeded(dynamic_cast<const DieRollBoolEvaluator*>(spec.eval)->getBoolResult());
        }

        updateReRollOptions(spec);
        upd = p->dieRollUpdate(spec.data);
        cont = upd.clueBurnAmount() > 0 || !upd.dieRollOptionId().isEmpty();
    } while (cont);

    //p->dieRollFinish(spec.data);
    gGame->context().setDieRoll(NULL);

    // Clean up options
    for (auto o : spec.options) {
        if (auto dro = dynamic_cast<DieRollOption*>(o)) {
            dro->reset();
        }
    }

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
    addDieRollOptions(spec, c, skill);
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
    ModifiedPropertyValue successMin = gGame->context().getCharacterProperty(c, PropertyValue::Prop_MinSuccessDieRoll);
    spec.data.addGeneralModifications(successMin.modifiers().toPropertyModificationDataList());
    for (int i = successMin.finalVal(); i <= 6; ++i) {
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

void DieTestHelper::finalize(DieTestHelper::DieTestSpec &spec, const QString &desc, const QString &sourceId)
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
    spec.data.setDescription(desc);
    spec.data.setSourceId(sourceId);
}

void DieTestHelper::updateReRollOptions(DieTestHelper::DieTestSpec &spec)
{
    QList<DieRollTestData::OptionDescription> descs;
    QList<GameOption *> &opts = spec.options;
    for (QList<GameOption *>::iterator it = opts.begin(); it != opts.end(); ) {
        if (!(*it)->isAvailable()) {
            if (DieRollOption *op = dynamic_cast<DieRollOption *> (*it)) op->reset();
            it = opts.erase(it);
        } else {
            descs.append(qMakePair((*it)->id(), (*it)->name()));
            ++it;
        }
    }
    spec.data.setRollOptions(descs);
}
