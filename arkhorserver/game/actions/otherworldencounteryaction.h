#ifndef OTHERWORLDENCOUNTERYACTION_H
#define OTHERWORLDENCOUNTERYACTION_H

#include "../gameaction.h"
#include "../gameoption.h"

class OtherWorldEncounteryAction : public GameAction
{
public:
    OtherWorldEncounteryAction();

    virtual AH::GamePhases phases() const { return AH::OtherWorldEncountery; }

    virtual bool execute();

    virtual QString name() const { return "Other World Encoutnery"; }
    virtual QString description() const { return ""; }
};

class OtherWorldEncounteryOption : public GameOption
{
public:
    OtherWorldEncounteryOption()
        : GameOption(&ea, AH::CannotContinue, AH::ChooseMandatory)
    {}

    virtual bool isAvailable();

private:
    OtherWorldEncounteryAction ea;
};

#endif // OTHERWORLDENCOUNTERYACTION_H
