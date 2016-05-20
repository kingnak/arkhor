#ifndef UNDELAYACTION_H
#define UNDELAYACTION_H

#include "../gameaction.h"
#include "../gameoption.h"

class UndelayAction : public GameAction
{
public:
    UndelayAction();

    bool execute();
    virtual QString notificationString(NotificationPart part, const QString &desc = QString::null) const;

    AH::GamePhases phases() const { return AH::Movement; }

    QString id() const { return "AC_UNDELAY"; }
    QString name() const { return "Undelay"; }
    QString description() const { return "You are no longer delayed"; }
};

class UndelayOption : public GameOption
{
public:
    UndelayOption()
        : GameOption(&ua, AH::CannotContinue, AH::ChooseMandatory, AH::Movement)
    { }

    QString id() const { return "OP_UNDELAY"; }

    virtual bool isAvailable() const;

private:
    UndelayAction ua;
};

#endif // UNDELAYACTION_H
