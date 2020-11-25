#ifndef ATTACKANCIENTONEPHASE_H
#define ATTACKANCIENTONEPHASE_H

#include "game/gamephase.h"
#include "game/gameaction.h"
#include "game/gameoption.h"
#include "game/phases/fight/chooseweaponsaction.h"
#include "game/phases/fight/attackaction.h"

class AttackAncientOneAction : public AttackAction
{
public:
    AttackAncientOneAction() : AttackAction(NULL)
    {
        setId("AC_ATTACK_ANCIENT_ONE");
    }

    bool execute() override;

protected:
    PropertyModificationList getMonsterModifications(Character *c) const override;
};

////////////////////////////

class AttackAncientOneOption : public GameOption
{
public:
    AttackAncientOneOption() : GameOption(&aaoa, AH::CannotContinue, AH::ChooseMandatory, AH::EndFightPhase) {}
private:
    AttackAncientOneAction aaoa;
};

////////////////////////////

class AttackAncientOnePhase : public GamePhase
{
public:
    AttackAncientOnePhase(Game *g);
    ~AttackAncientOnePhase() override;

protected:
    QList<GameOption *> getPhaseOptions() override;

private:
    AttackAncientOneOption *m_attack;
    ChooseWeaponsOption *m_choose;
};

#endif // ATTACKANCIENTONEPHASE_H
