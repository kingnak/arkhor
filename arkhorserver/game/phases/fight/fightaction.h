#ifndef FIGHTACTION_H
#define FIGHTACTION_H

#include "game/gameaction.h"
#include "game/gameoption.h"

class FightPhase;

class FightAction : public GameAction
{
public:
    FightAction(FightPhase *fight);

    QString id() const override { return "AC_FIGHT"; }

    AH::GamePhases phases() const override { return AH::Movement; }

    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    QString name() const override { return "Fight"; }
    QString description() const override { return ""; }

private:
    FightPhase *m_fight;
};

class FightOption : public GameOption
{
public:
    FightOption(FightPhase *fight) :
        GameOption(&fa, AH::CanContinue, AH::ChooseMandatory, AH::Movement),
        fa(fight)
    {}

    QString id() const override { return "OP_FIGHT"; }
    QString sourceId() const override;

    bool isAvailable() const override;

private:
    FightAction fa;
};

#endif // FIGHTACTION_H
