#ifndef HORRORACTION_H
#define HORRORACTION_H

#include "game/gameoption.h"
#include "game/gameaction.h"

class FightPhase;

class HorrorAction : public GameAction
{
public:
    HorrorAction(FightPhase *fight);

    QString id() const override { return "AC_HORROR"; }

    AH::GamePhases phases() const override { return AH::Movement; }

    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    QString name() const override { return "Horror Test"; }
    QString description() const override { return ""; }

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

    QString id() const override { return "OP_HORROR"; }
    QString sourceId() const override;

    AutoChoose autoChoose() const override { return AutoChoose::Possible; }
    bool isAvailable() const override;
    AH::Common::ModifiedPropertyValueData baseProperty() const override;

private:
    HorrorAction ha;
};

#endif // HORRORACTION_H
