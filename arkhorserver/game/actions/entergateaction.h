#ifndef ENTERGATEACTION_H
#define ENTERGATEACTION_H

#include "../gameaction.h"
#include "../gameoption.h"

class EnterGateAction : public GameAction
{
public:
    EnterGateAction();

    virtual AH::GamePhases phases() const { return AH::ArkhamEncountery; }

    virtual bool execute();

    virtual QString name() const { return "Enter Gate"; }
    virtual QString description() const { return ""; }
};

class EnterGateOption : public GameOption
{
public:
    EnterGateOption()
        : GameOption(&ea, AH::CannotContinue, AH::ChooseMandatory)
    {}

    virtual bool isAvailable();

private:
    EnterGateAction ea;
};

#endif // ENTERGATEACTION_H
