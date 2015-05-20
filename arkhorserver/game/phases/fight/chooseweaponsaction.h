#ifndef CHOOSEWEAPONSACTION_H
#define CHOOSEWEAPONSACTION_H

#include "game/gameaction.h"
#include "game/gameoption.h"

class FightPhase;

class ChooseWeaponsAction : public GameAction
{
public:
    ChooseWeaponsAction(FightPhase *fight);

    QString id() const { return "AC_CHOOSE_WEAPONS"; }

    virtual AH::GamePhases phases() const { return AH::Movement; }
    virtual bool execute();

    virtual QString name() const { return "Choose Weapons"; }
    virtual QString description() const { return ""; }

private:
    FightPhase *m_fight;
    friend class ChooseWeaponsOption;
};

class ChooseWeaponsOption : public GameOption
{
public:
    ChooseWeaponsOption(FightPhase *fight)
        : GameOption(&cwa, AH::CanContinue, AH::ChooseMandatory, AH::Movement),
          cwa(fight)
    {}
    QString id() const { return "OP_CHOOSE_WEAPONS"; }

    virtual bool isAvailable() { return true; }

private:
    ChooseWeaponsAction cwa;
};

#endif // CHOOSEWEAPONSACTION_H
