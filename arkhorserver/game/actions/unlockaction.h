#ifndef UNLOCKACTION_H
#define UNLOCKACTION_H

#include "../gameaction.h"

class UnlockAction : public GameAction
{
public:
    UnlockAction();
    AH::GamePhases phases() const override;
    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    QString id() const override { return "AC_UNLOCK"; }
    QString name() const override { return "Unlock"; }
    QString description() const override { return ""; }
};

#endif // UNLOCKACTION_H
