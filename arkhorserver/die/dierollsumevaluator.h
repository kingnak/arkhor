#ifndef DIEROLLSUMEVALUATOR_H
#define DIEROLLSUMEVALUATOR_H

#include "die/dierollevaluator.h"

class DieRollSumEvaluator : public DieRollEvaluator
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

class DieRollSumBoolEvaluator : public DieRollBoolEvaluator
{
public:
    DieRollSumBoolEvaluator(DiePool initialPool, quint32 target, EvaluationType type = GREATER);

    virtual bool getBoolResult() const;

    // forwarders:
    virtual void evaluate();
    virtual qint32 getResult() const;

    virtual void rerollAll() = 0;
    virtual void rollNew() = 0;

    virtual const DiePool *pool() const { return m_eval.pool(); }
    virtual void addDie(Die *die);
    virtual void addDice(QList<StandardDieSpec> specs);

private:
    DieRollSumEvaluator m_eval;
};

#endif // DIEROLLSUMEVALUATOR_H
