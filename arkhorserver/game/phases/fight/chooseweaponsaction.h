#ifndef CHOOSEWEAPONSACTION_H
#define CHOOSEWEAPONSACTION_H

#include "game/gameaction.h"
#include "game/gameoption.h"

class FightPhase;
class GameObject;

class ChooseWeaponsAction : public GameAction
{
public:
    ChooseWeaponsAction(FightPhase *fight);

    QString id() const override { return "AC_CHOOSE_WEAPONS"; }

    AH::GamePhases phases() const override { return AH::Movement; }
    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    QString name() const override { return "Choose Weapons"; }
    QString description() const override { return ""; }

private:
    bool doEquip(const QList<GameObject *> &oldWeapons, const QStringList &newWeapons, int hands);

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
    QString id() const override { return "OP_CHOOSE_WEAPONS"; }

    bool isAvailable() const override { return true; }

private:
    ChooseWeaponsAction cwa;
};

#endif // CHOOSEWEAPONSACTION_H
