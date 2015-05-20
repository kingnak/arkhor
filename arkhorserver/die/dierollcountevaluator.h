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

private:
    const QSet<quint32> m_successRolls;
    QSet<DiePool::DiePoolIndex> m_failed;
    qint32 m_resultValue;
    DiePool m_pool;
};

class DieRollCountBoolEvaluator : public DieRollBoolEvaluator
{
public:
    DieRollCountBoolEvaluator(DiePool initialPool, const QSet<quint32> &successRolls, quint32 target, EvaluationType type = GREATER);
    ~DieRollCountBoolEvaluator() {}

    virtual bool getBoolResult() const;


    // forwarders:
    virtual void evaluate();
    virtual qint32 getResult() const;

    virtual void rerollAll();
    virtual void rollNew();

    virtual const DiePool *pool() const { return m_eval.pool(); }
    virtual void addDie(Die *die);
    virtual void addDice(QList<StandardDieSpec> specs);

    void rerollFailed();

private:
    DieRollCountEvaluator m_eval;
};

#endif // DIEROLLCOUNTEVALUATOR_H
