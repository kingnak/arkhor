#ifndef ATTACKANCIENTONEPHASE_H
#define ATTACKANCIENTONEPHASE_H

#include "game/gamephase.h"
#include "game/gameaction.h"
#include "game/gameoption.h"
#include "game/phases/fight/chooseweaponsaction.h"

class AttackAncientOneAction : public GameAction
{
public:
    AttackAncientOneAction() { setId("AC_ATTACK_ANCIENT_ONE"); }
    virtual bool execute();
};

class AttackAncientOneOption : public GameOption
{
public:
    AttackAncientOneOption() : GameOption(&aaoa, AH::CannotContinue, AH::ChooseMandatory, AH::EndFightPhase) {}
private:
    AttackAncientOneAction aaoa;
};

class AttackAncientOnePhase : public GamePhase
{
public:
    AttackAncientOnePhase(Game *g);
    ~AttackAncientOnePhase();

protected:
    QList<GameOption *> getPhaseOptions();

private:
    AttackAncientOneOption *m_attack;
    ChooseWeaponsOption *m_choose;
};

#endif // ATTACKANCIENTONEPHASE_H
