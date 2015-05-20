#ifndef FOCUSACTION_H
#define FOCUSACTION_H

#include "../gameaction.h"
#include "../gameoption.h"
#include <QString>

class FocusAction : public GameAction
{
public:
    FocusAction();

    bool execute();
    AH::GamePhases phases() const { return AH::Upkeep; }

    QString id() const { return "AC_FOCUS"; }
    QString name() const { return "Focus"; }
    QString description() const { return "Set the focus attributes"; }
};

class FocusOption : public GameOption
{
public:
    FocusOption()
        : GameOption(&fa, AH::CanContinue, AH::ChooseOptional, AH::Upkeep)
    { }

    QString id() const { return "OP_FOCUS"; }

    virtual bool isAvailable();

private:
    FocusAction fa;
};

#endif // FOCUSACTION_H
