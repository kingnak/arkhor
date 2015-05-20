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
    virtual void addDie(Die *die);

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


    virtual void addDie(Die *die);

private:
    DieRollSumEvaluator m_eval;
};

#endif // DIEROLLSUMEVALUATOR_H
