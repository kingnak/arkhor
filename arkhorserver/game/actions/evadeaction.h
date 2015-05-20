#ifndef EVADEACTION_H
#define EVADEACTION_H

#include "../gameaction.h"
#include "../gameoption.h"

class EvadeAction : public GameAction
{
public:
    EvadeAction();

    QString id() const { return "AC_EVADE"; }

    virtual AH::GamePhases phases() const { return AH::Movement; }
    virtual bool execute();

    virtual QString name() const { return "Sneak"; }
    virtual QString description() const { return ""; }

private:
    bool m_hasFailedAttempt;
    friend class EvadeOption;
};

class EvadeOption : public GameOption
{
public:
    EvadeOption()
        : GameOption(&sa, AH::CanContinue, AH::ChooseMandatory, AH::Movement)
    {}
    QString id() const { return "OP_EVADE"; }

    void reset() { sa.m_hasFailedAttempt = false; }

    virtual bool isAvailable();

private:
    EvadeAction sa;
};

#endif // EVADEACTION_H
