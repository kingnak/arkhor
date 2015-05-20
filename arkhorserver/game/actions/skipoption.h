#ifndef SKIPOPTION_H
#define SKIPOPTION_H

#include "../gameoption.h"
#include "game/game.h"

class SkipOption : public GameOption {
public:
    SkipOption()
        : GameOption(NULL, AH::CannotContinue, AH::ChooseOptional, AH::AllPhases)
    {
    }

    virtual QString id() const { return "OP_SKIP"; }
    virtual QString name() const { return "Skip"; }
    virtual QString description() const { return "Finish this round."; }

    virtual bool isAvailable() const { return true; }

    virtual bool execute() { return true; }
};

#endif // SKIPOPTION_H
