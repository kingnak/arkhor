#ifndef UNTAPACTION_H
#define UNTAPACTION_H

#include "../gameaction.h"

class UntapAction : public GameAction
{
public:
    UntapAction();
    virtual AH::GamePhases phases() const;
    virtual bool execute();
    virtual QString notificationString(NotificationPart part, const QString &desc = QString::null) const;

    QString id() const { return "AC_UNTAP"; }
    virtual QString name() const { return "Untap"; }
    virtual QString description() const { return ""; }
};

#endif // UNTAPACTION_H
