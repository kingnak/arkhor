#ifndef ENTERGATEACTION_H
#define ENTERGATEACTION_H

#include "../gameaction.h"
#include "../gameoption.h"

class EnterGateAction : public GameAction
{
public:
    EnterGateAction();

    QString id() const { return "AC_ENTER_GATE"; }

    virtual AH::GamePhases phases() const { return AH::ArkhamEncountery; }

    virtual bool execute();

    virtual QString name() const { return "Enter Gate"; }
    virtual QString description() const { return ""; }
};

class EnterGateOption : public GameOption
{
public:
    EnterGateOption()
        : GameOption(&ea, AH::CannotContinue, AH::ChooseMandatory, AH::ArkhamEncountery)
    {}

    QString id() const { return "OP_ENTER_GATE"; }

    virtual bool isAvailable();

private:
    EnterGateAction ea;
};

#endif // ENTERGATEACTION_H
