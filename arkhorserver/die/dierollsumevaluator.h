#ifndef DIEROLLSUMEVALUATOR_H
#define DIEROLLSUMEVALUATOR_H

#include "die/dierollevaluator.h"

class DieRollSumEvaluator : public virtual DieRollEvaluator
{
public:
    DieRollSumEvaluator(DiePool initialPool);
    ~DieRollSumEvaluator() {}

    virtual void evaluate();
    virtual qint32 getResult() const;

    virtual void rerollAll();
    virtual void rollNew();

    // forwarders:
    virtual const DiePool *pool() const { return &m_pool; }
    virtual void addDie(Die *die);
    virtual void addDice(QList<StandardDieSpec> specs);

private:
    DiePool m_pool;
    qint32 m_resultValue;
};

#pragma warning(push)
#pragma warning(disable:4250) // Inherits method via dominance
class DieRollSumBoolEvaluator : public DieRollBoolEvaluator, public DieRollSumEvaluator
{
public:
    DieRollSumBoolEvaluator(DiePool initialPool, quint32 target, EvaluationType type = GREATER);

    virtual bool getBoolResult() const;
};
#pragma warning(pop)

#endif // DIEROLLSUMEVALUATOR_H
