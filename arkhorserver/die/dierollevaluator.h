#ifndef DIEROLLEVALUATOR_H
#define DIEROLLEVALUATOR_H

#include "die/dierollresult.h"

class DieRollEvaluator
{
public:
    virtual void evaluate() = 0;
    virtual qint32 getResult() const = 0;

    virtual void rerollAll() = 0;
    virtual void rollNew() = 0;

    // forwarders:
    virtual const DiePool *pool() const = 0;
    virtual void addDie(Die *die) = 0;
    virtual void addDice(QList<StandardDieSpec> specs) = 0;

    virtual ~DieRollEvaluator() = default;

protected:
    DieRollEvaluator() = default;
};

class DieRollBoolEvaluator : public virtual DieRollEvaluator
{
public:
    enum EvaluationType {
        EQUALS,
        LESS,
        LESS_EQUALS,
        GREATER,
        GREATER_EQUALS
    };

    DieRollBoolEvaluator(quint32 target, EvaluationType type = GREATER)
        : m_target(target), m_evaluationType(type) {}

    virtual bool getBoolResult() const = 0;
    virtual void succeed() = 0;

protected:
    bool boolEvaluate(quint32 value) const;

    const quint32 m_target;
    const EvaluationType m_evaluationType;
};

#endif // DIEROLLEVALUATOR_H
