#ifndef UNDELAYACTION_H
#define UNDELAYACTION_H

#include "../gameaction.h"
#include "../gameoption.h"

class UndelayAction : public GameAction
{
public:
    UndelayAction();

    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    AH::GamePhases phases() const override { return AH::Movement; }

    QString id() const override { return "AC_UNDELAY"; }
    QString name() const override { return "Undelay"; }
    QString description() const override { return "You are no longer delayed"; }
};

class UndelayOption : public GameOption
{
public:
    UndelayOption()
        : GameOption(&ua, AH::CannotContinue, AH::ChooseMandatory, AH::Movement)
    { }

    QString id() const override { return "OP_UNDELAY"; }

    bool isAvailable() const override;

private:
    UndelayAction ua;
};

#endif // UNDELAYACTION_H
