#ifndef EVADEACTION_H
#define EVADEACTION_H

#include "game/gameaction.h"
#include "game/gameoption.h"

class FightPhase;

class EvadeAction : public GameAction
{
public:
    EvadeAction(FightPhase *fight);

    QString id() const { return "AC_EVADE"; }

    virtual AH::GamePhases phases() const { return AH::Movement; }
    virtual bool execute();

    //virtual QString name() const { return "Evade"; }
    virtual QString description() const { return ""; }

private:
    bool m_hasFailedAttempt;
    FightPhase *m_fight;
    friend class EvadeOption;
};

class EvadeOption : public GameOption
{
public:
    EvadeOption(FightPhase *fight)
        : GameOption(&sa, AH::CanContinue, AH::ChooseMandatory, AH::Movement),
          sa(fight)
    {}
    QString id() const { return "OP_EVADE"; }

    void reset() { sa.m_hasFailedAttempt = false; }
    void setName(QString name) {
        sa.m_name = name;
    }

    virtual bool isAvailable();

private:
    EvadeAction sa;
};

#endif // EVADEACTION_H
