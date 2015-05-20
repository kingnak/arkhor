#ifndef FIGHTACTION_H
#define FIGHTACTION_H

#include "../gameaction.h"
#include "../gameoption.h"

class FightAction : public GameAction
{
public:
    FightAction();

    QString id() const { return "AC_FIGHT"; }

    virtual AH::GamePhases phases() const { return AH::Movement; }

    virtual bool execute();

    virtual QString name() const { return "Fight"; }
    virtual QString description() const { return ""; }
};

class FightOption : public GameOption
{
public:
    FightOption() :
        GameOption(&fa, AH::CanContinue, AH::ChooseMandatory, AH::Movement)
    {}

    QString id() const { return "OP_FIGHT"; }

    virtual bool isAvailable();

private:
    FightAction fa;
};

#endif // FIGHTACTION_H
