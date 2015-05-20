#include "die/dierollsumevaluator.h"

DieRollSumEvaluator::DieRollSumEvaluator(DiePool initialPool)
    : m_pool(initialPool), m_resultValue(0)
{
}

void DieRollSumEvaluator::evaluate()
{
    m_resultValue = 0;
    DieRollResult result = m_pool.getResult();
    foreach (DieRollResultItem i, result) {
        m_resultValue += i.value();
    }
}

qint32 DieRollSumEvaluator::getResult() const
{
    return m_resultValue;
}

void DieRollSumEvaluator::rerollAll()
{
    m_pool.unroll();
    m_pool.roll();
    evaluate();
}

void DieRollSumEvaluator::rollNew()
{
    m_pool.roll();
    evaluate();
}

void DieRollSumEvaluator::addDie(Die *die)
{
    m_pool.addDie(die);
}

void DieRollSumEvaluator::addDice(QList<StandardDieSpec> specs)
{
    m_pool.addDice(specs);
}


////////////////////////////////////////////////////

DieRollSumBoolEvaluator::DieRollSumBoolEvaluator(DiePool initialPool, quint32 target, DieRollBoolEvaluator::EvaluationType type)
    : DieRollBoolEvaluator(target, type), m_eval(initialPool)
{

}

bool DieRollSumBoolEvaluator::getBoolResult() const
{
    return boolEvaluate(getResult());
}

void DieRollSumBoolEvaluator::evaluate()
{
    m_eval.evaluate();
}

qint32 DieRollSumBoolEvaluator::getResult() const
{
    return m_eval.getResult();
}

void DieRollSumBoolEvaluator::rerollAll()
{
    m_eval.rerollAll();
}

void DieRollSumBoolEvaluator::rollNew()
{
    m_eval.rollNew();
}

void DieRollSumBoolEvaluator::addDie(Die *die)
{
    m_eval.addDie(die);
}

void DieRollSumBoolEvaluator::addDice(QList<StandardDieSpec> specs)
{
    m_eval.addDice(specs);
}
