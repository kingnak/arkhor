#ifndef ATTACKACTION_H
#define ATTACKACTION_H

#include "game/gameaction.h"
#include "game/gameoption.h"

class FightPhase;
class PropertyModificationList;

class AttackAction : public GameAction
{
public:
    AttackAction(FightPhase *fight);

    QString id() const { return "AC_ATTACK"; }

    virtual AH::GamePhases phases() const { return AH::Movement; }
    virtual bool execute();

    virtual QString name() const { return "Attack"; }
    virtual QString description() const { return ""; }

protected:
    void filterEquipped(PropertyModificationList &lst);
    void discardAfterAttack(PropertyModificationList &lst);

    FightPhase *m_fight;
    friend class AttackOption;
};

class AttackOption : public GameOption
{
public:
    AttackOption(FightPhase *fight)
        : GameOption(&aa, AH::CanContinue, AH::ChooseMandatory, AH::Movement),
          aa(fight)
    {}

    QString id() const { return "OP_ATTACK"; }
    QString sourceId() const;

    virtual bool isAvailable() const { return true; }

private:
    AttackAction aa;
};


#endif // ATTACKACTION_H
