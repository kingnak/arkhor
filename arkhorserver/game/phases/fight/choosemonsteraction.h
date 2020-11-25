#ifndef CHOOSEMONSTERACTION_H
#define CHOOSEMONSTERACTION_H

#include "game/gameaction.h"
#include "game/gameoption.h"

class Monster;
class FightPhase;

class ChooseMonsterAction : public GameAction
{
public:
    ChooseMonsterAction(FightPhase *fight);

    QString id() const override { return "AC_CHOOSE_MONSTER"; }

    AH::GamePhases phases() const override { return AH::Movement; }
    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    QString name() const override { return "Choose Monster"; }
    QString description() const override { return "Choose Monster to fight"; }

private:
    FightPhase *m_fight;
    QList<Monster*> m_monsters;
    friend class ChooseMonsterOption;
};

class ChooseMonsterOption : public GameOption
{
public:
    ChooseMonsterOption(FightPhase *fight)
        : GameOption(&cma, AH::CanContinue, AH::ChooseMandatory, AH::Movement),
          cma(fight)
    {}
    QString id() const override { return "OP_CHOOSE_MONSTER"; }

    AutoChoose autoChoose() const override { return AutoChoose::Always; }
    bool isAvailable() const override { return true; }
    void setMonsters(QList<Monster*> monsters) {
        cma.m_monsters = monsters;
    }

private:
    ChooseMonsterAction cma;
};

#endif // CHOOSEMONSTERACTION_H
