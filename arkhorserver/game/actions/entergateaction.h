#ifndef ENTERGATEACTION_H
#define ENTERGATEACTION_H

#include "../gameaction.h"
#include "../gameoption.h"

class EnterGateAction : public GameAction
{
public:
    EnterGateAction();

    QString id() const override { return "AC_ENTER_GATE"; }

    AH::GamePhases phases() const override { return AH::ArkhamEncountery; }

    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    QString name() const override { return "Enter Gate"; }
    QString description() const override { return ""; }
};

class EnterGateOption : public GameOption
{
public:
    EnterGateOption()
        : GameOption(&ea, AH::CannotContinue, AH::ChooseMandatory, AH::ArkhamEncountery)
    {}

    QString id() const override { return "OP_ENTER_GATE"; }

    bool isAvailable() const override;

private:
    EnterGateAction ea;
};

#endif // ENTERGATEACTION_H
