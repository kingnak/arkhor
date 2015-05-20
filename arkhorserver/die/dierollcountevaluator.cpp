#include "die/dierollcountevaluator.h"

DieRollCountEvaluator::DieRollCountEvaluator(DiePool initialPool, const QSet<quint32> &successRolls)
    : m_successRolls(successRolls), m_resultValue(0), m_pool(initialPool)
{
}

void DieRollCountEvaluator::evaluate()
{
    m_resultValue = 0;
    m_failed.clear();
    DieRollResult result = m_pool.getResult();
    foreach (DieRollResultItem i, result) {
        if ( m_successRolls.contains(i.value()) ) {
            m_resultValue++;
        } else {
            m_failed << i.index();
        }
    }
}

qint32 DieRollCountEvaluator::getResult() const
{
    return m_resultValue;
}

void DieRollCountEvaluator::rerollAll()
{
    m_pool.unroll();
    m_pool.roll();
    evaluate();
}

void DieRollCountEvaluator::rollNew()
{
    m_pool.roll();
    evaluate();
}

void DieRollCountEvaluator::addDie(Die *die)
{
    m_pool.addDie(die);
}

void DieRollCountEvaluator::rerollFailed()
{
    m_pool.reroll(m_failed);
    evaluate();
}

////////////////////////////////////////////////////

DieRollCountBoolEvaluator::DieRollCountBoolEvaluator(DiePool initialPool, const QSet<quint32> &successRolls, quint32 target, DieRollBoolEvaluator::EvaluationType type)
    : DieRollBoolEvaluator(target, type), m_eval(initialPool, successRolls)
{

}

bool DieRollCountBoolEvaluator::getBoolResult() const
{
    return boolEvaluate(getResult());
}

void DieRollCountBoolEvaluator::evaluate()
{
    m_eval.evaluate();
}

qint32 DieRollCountBoolEvaluator::getResult() const
{
    return m_eval.getResult();
}

void DieRollCountBoolEvaluator::rerollAll()
{
    m_eval.rerollAll();
}

void DieRollCountBoolEvaluator::rollNew()
{
    m_eval.rollNew();
}

void DieRollCountBoolEvaluator::addDie(Die *die)
{
    m_eval.addDie(die);
}

void DieRollCountBoolEvaluator::rerollFailed()
{
    m_eval.rerollFailed();
}
