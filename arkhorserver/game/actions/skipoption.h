#ifndef SKIPOPTION_H
#define SKIPOPTION_H

#include "../gameoption.h"

class SkipOption : public GameOption {
public:
    SkipOption()
        : GameOption(NULL, AH::CannotContinue, AH::ChooseOptional, AH::AllPhases)
    {}

    virtual bool isAvailable() { return true; }

    virtual bool execute() { return true; }
};

#endif // SKIPOPTION_H
