#ifndef HORRORACTION_H
#define HORRORACTION_H

#include "game/gameoption.h"
#include "game/gameaction.h"

class FightPhase;

class HorrorAction : public GameAction
{
public:
    HorrorAction(FightPhase *fight);

    QString id() const { return "AC_HORROR"; }

    virtual AH::GamePhases phases() const { return AH::Movement; }

    virtual bool execute();

    virtual QString name() const { return "Horror Test"; }
    virtual QString description() const { return ""; }

private:
    FightPhase *m_fight;
};

class HorrorOption : public GameOption
{
public:
    HorrorOption(FightPhase *fight) :
        GameOption(&ha, AH::CanContinue, AH::ChooseMandatory, AH::Movement),
        ha(fight)
    {}

    QString id() const { return "OP_HORROR"; }

    virtual bool isAvailable() const { return true; }

private:
    HorrorAction ha;
};

#endif // HORRORACTION_H
