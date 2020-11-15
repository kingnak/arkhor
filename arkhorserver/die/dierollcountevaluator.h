#ifndef DIEROLLCOUNTEVALUATOR_H
#define DIEROLLCOUNTEVALUATOR_H

#include "die/dierollevaluator.h"

class DieRollCountEvaluator : public virtual DieRollEvaluator
{
public:
    DieRollCountEvaluator(DiePool initialPool, const QSet<quint32> &successRolls);
    ~DieRollCountEvaluator() {}

    virtual void evaluate();
    virtual qint32 getResult() const;

    virtual void rerollAll();
    virtual void rollNew();

    // forwarders:
    virtual const DiePool *pool() const { return &m_pool; }
    virtual void addDie(Die *die);
    virtual void addDice(QList<StandardDieSpec> specs);

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
    DieRollCountBoolEvaluator(DiePool initialPool, const QSet<quint32> &successRolls, quint32 target, EvaluationType type = GREATER);
    ~DieRollCountBoolEvaluator() {}

    virtual bool getBoolResult() const override;
    void succeed() override;
};
#pragma warning(pop)

#endif // DIEROLLCOUNTEVALUATOR_H
