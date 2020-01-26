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

    QString id() const { return "AC_CHOOSE_MONSTER"; }

    virtual AH::GamePhases phases() const { return AH::Movement; }
    virtual bool execute();
    virtual QString notificationString(NotificationPart part, const QString &desc = QString::null) const;

    virtual QString name() const { return "Choose Monster"; }
    virtual QString description() const { return "Choose Monster to fight"; }

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
    QString id() const { return "OP_CHOOSE_MONSTER"; }

    virtual AutoChoose autoChoose() const { return AutoChoose::Always; }
    virtual bool isAvailable() const { return true; }
    void setMonsters(QList<Monster*> monsters) {
        cma.m_monsters = monsters;
    }

private:
    ChooseMonsterAction cma;
};

#endif // CHOOSEMONSTERACTION_H
