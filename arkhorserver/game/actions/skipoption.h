#ifndef SKIPOPTION_H
#define SKIPOPTION_H

#include "../gameoption.h"
#include "game/game.h"

class SkipOption : public GameOption {
public:
    SkipOption()
        : GameOption(NULL, AH::CannotContinue, AH::ChooseOptional, AH::AllPhases)
    {
        m_id = "OP_SKIP";
    }

    //virtual QString id() const { return "OP_SKIP"; }
    virtual QString name() const { return "Finish"; }
    virtual QString description() const { return "Finish your turn."; }

    virtual bool isAvailable() const { return true; }

    virtual bool execute() { return true; }
};

#endif // SKIPOPTION_H
