#include "die/dierollsumevaluator.h"

DieRollSumEvaluator::DieRollSumEvaluator(const DiePool &initialPool)
    : m_pool(initialPool)
    , m_resultValue(0)
{
}

void DieRollSumEvaluator::evaluate()
{
    m_resultValue = 0;
    DieRollResult result = m_pool.getResult();
    for (const auto &i : result) {
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

DieRollSumBoolEvaluator::DieRollSumBoolEvaluator(const DiePool &initialPool, quint32 target, DieRollBoolEvaluator::EvaluationType type)
    : DieRollBoolEvaluator(target, type)
    , DieRollSumEvaluator(initialPool)
{

}

bool DieRollSumBoolEvaluator::getBoolResult() const
{
    return boolEvaluate(getResult());
}
