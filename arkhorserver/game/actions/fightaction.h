#ifndef FIGHTACTION_H
#define FIGHTACTION_H

#include "../gameaction.h"
#include "../gameoption.h"

class FightAction : public GameAction
{
public:
    FightAction();

    virtual AH::GamePhases phases() const { return AH::Movement; }

    virtual bool execute();

    virtual QString name() const { return "Fight"; }
    virtual QString description() const { return ""; }
};

class FightOption : public GameOption
{
public:
    FightOption() :
        GameOption(&fa, AH::CanContinue, AH::ChooseMandatory)
    {}

    virtual bool isAvailable();

private:
    FightAction fa;
};

#endif // FIGHTACTION_H
