#ifndef SETOUTACTION_H
#define SETOUTACTION_H

#include <game/gameaction.h>

class SetOutAction : public GameAction
{
public:
    SetOutAction();
    virtual AH::GamePhases phases() const { return AH::Upkeep; }
    virtual bool execute();
    virtual QString notificationString(NotificationPart part, const QString &desc = QString::null) const;

    QString id() const { return "AC_SETOUT"; }
    virtual QString name() const { return "Setout"; }
    virtual QString description() const { return ""; }
};

#endif // SETOUTACTION_H
