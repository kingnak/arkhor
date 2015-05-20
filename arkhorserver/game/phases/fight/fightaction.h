#ifndef FIGHTACTION_H
#define FIGHTACTION_H

#include "game/gameaction.h"
#include "game/gameoption.h"

class FightPhase;

class FightAction : public GameAction
{
public:
    FightAction(FightPhase *fight);

    QString id() const { return "AC_FIGHT"; }

    virtual AH::GamePhases phases() const { return AH::Movement; }

    virtual bool execute();

    virtual QString name() const { return "Fight"; }
    virtual QString description() const { return ""; }

private:
    FightPhase *m_fight;
};

class FightOption : public GameOption
{
public:
    FightOption(FightPhase *fight) :
        GameOption(&fa, AH::CanContinue, AH::ChooseMandatory, AH::Movement),
        fa(fight)
    {}

    QString id() const { return "OP_FIGHT"; }
    QString sourceId() const;

    virtual bool isAvailable() const;

private:
    FightAction fa;
};

#endif // FIGHTACTION_H
