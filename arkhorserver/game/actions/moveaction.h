#ifndef MOVEACTION_H
#define MOVEACTION_H

#include "../gameaction.h"
#include "../gameoption.h"

class MoveAction : public GameAction
{
public:
    MoveAction();

    QString id() const { return "AC_MOVE"; }

    virtual AH::GamePhases phases() const { return AH::Movement; }

    virtual bool execute();

    virtual QString name() const { return "Move"; }
    virtual QString description() const { return ""; }

private:
    bool moveArkham();
    bool moveOtherWorld();
};

class MoveOption : public GameOption
{
public:
    MoveOption() : GameOption(&ma, AH::CanContinue, AH::ChooseOptional, AH::Movement) {}

    QString id() const { return "OP_MOVE"; }

    void determineMovementType();

    virtual bool isAvailable();

private:
    MoveAction ma;
};

#endif // MOVEACTION_H
