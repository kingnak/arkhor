#ifndef UNLOCKACTION_H
#define UNLOCKACTION_H

#include "../gameaction.h"

class UnlockAction : public GameAction
{
public:
    UnlockAction();
    virtual AH::GamePhases phases() const;
    virtual bool execute();
    virtual QString notificationString(NotificationPart part, const QString &desc = QString::null) const;

    QString id() const { return "AC_UNLOCK"; }
    virtual QString name() const { return "Unlock"; }
    virtual QString description() const { return ""; }
};

#endif // UNLOCKACTION_H
