#ifndef DIEROLLSUMEVALUATOR_H
#define DIEROLLSUMEVALUATOR_H

#include "die/dierollevaluator.h"

class DieRollSumEvaluator : public virtual DieRollEvaluator
{
public:
    DieRollSumEvaluator(const DiePool &initialPool);
    ~DieRollSumEvaluator() override = default;

    void evaluate() override;
    qint32 getResult() const override;

    void rerollAll() override;
    void rollNew() override;

    // forwarders:
    const DiePool *pool() const override { return &m_pool; }
    void addDie(Die *die) override;
    void addDice(QList<StandardDieSpec> specs) override;

private:
    DiePool m_pool;
    qint32 m_resultValue;
};

#pragma warning(push)
#pragma warning(disable:4250) // Inherits method via dominance
class DieRollSumBoolEvaluator : public DieRollBoolEvaluator, public DieRollSumEvaluator
{
public:
    DieRollSumBoolEvaluator(const DiePool &initialPool, quint32 target, EvaluationType type = GREATER);

    bool getBoolResult() const override;
};
#pragma warning(pop)

#endif // DIEROLLSUMEVALUATOR_H
