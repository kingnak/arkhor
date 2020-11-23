#ifndef DIEROLLCOUNTEVALUATOR_H
#define DIEROLLCOUNTEVALUATOR_H

#include "die/dierollevaluator.h"

class DieRollCountEvaluator : public virtual DieRollEvaluator
{
public:
    DieRollCountEvaluator(const DiePool &initialPool, const QSet<quint32> &successRolls);
    ~DieRollCountEvaluator() override {}

    void evaluate() override;
    qint32 getResult() const override;

    void rerollAll() override;
    void rollNew() override;

    // forwarders:
    const DiePool *pool() const override { return &m_pool; }
    void addDie(Die *die) override;
    void addDice(QList<StandardDieSpec> specs) override;

    void rerollFailed();
    void rerollNumFailed(int amount);

protected:
    const QSet<quint32> m_successRolls;
    QSet<DiePool::DiePoolIndex> m_failed;
    qint32 m_resultValue;
    DiePool m_pool;
};

#pragma warning(push)
#pragma warning(disable:4250) // Inherits method via dominance
class DieRollCountBoolEvaluator : public DieRollBoolEvaluator, public DieRollCountEvaluator
{
public:
    DieRollCountBoolEvaluator(const DiePool &initialPool, const QSet<quint32> &successRolls, quint32 target, EvaluationType type = GREATER);
    ~DieRollCountBoolEvaluator() override {}

    bool getBoolResult() const override;
    void succeed() override;
};
#pragma warning(pop)

#endif // DIEROLLCOUNTEVALUATOR_H
