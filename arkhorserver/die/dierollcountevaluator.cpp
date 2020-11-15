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

void DieRollCountEvaluator::addDice(QList<StandardDieSpec> specs)
{
    m_pool.addDice(specs);
}

void DieRollCountEvaluator::rerollFailed()
{
    m_pool.reroll(m_failed);
    evaluate();
}

void DieRollCountEvaluator::rerollNumFailed(int amount)
{
    while (m_failed.size() > amount) {
        m_failed.erase(m_failed.begin());
    }
    m_pool.reroll(m_failed);
    evaluate();
}

////////////////////////////////////////////////////

DieRollCountBoolEvaluator::DieRollCountBoolEvaluator(DiePool initialPool, const QSet<quint32> &successRolls, quint32 target, DieRollBoolEvaluator::EvaluationType type)
    : DieRollBoolEvaluator(target, type), DieRollCountEvaluator(initialPool, successRolls)
{

}

bool DieRollCountBoolEvaluator::getBoolResult() const
{
    return boolEvaluate(getResult());
}

void DieRollCountBoolEvaluator::succeed()
{
    m_pool.clear();
    quint32 t = m_target;

    QList<Die*> dice;
    // Get values for fail and success die
    auto l = m_successRolls.toList();
    qSort(l);
    quint32 success = l.last();
    quint32 fail = l.first()-1;
    if (fail < 1) {
        fail = l.last()+1;
    }

    switch (m_evaluationType) {
    case LESS:
    case LESS_EQUALS:
        // 1 fail die will always be less then target
        dice << DieFactory::instance().createFixedDie(fail);
        break;

    case GREATER:
        t++;
        // fallthrough
    case GREATER_EQUALS:
    case EQUALS:
        // Add success dice until we reach target
        for (quint32 i = 0; i < t; ++i) {
            dice << DieFactory::instance().createFixedDie(success);
        }
        break;
    }

    m_pool.setDice(dice);
    evaluate();
}
