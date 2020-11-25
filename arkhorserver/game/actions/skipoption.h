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

    //QString id() const override { return "OP_SKIP"; }
    QString name() const override { return "Finish"; }
    QString description() const override { return "Finish your turn."; }

    AutoChoose autoChoose() const override { return AutoChoose::Always; }
    bool isAvailable() const override { return true; }

    bool execute() override { return true; }
};

#endif // SKIPOPTION_H
