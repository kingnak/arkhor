#ifndef SETOUTACTION_H
#define SETOUTACTION_H

#include <game/gameaction.h>

class SetOutAction : public GameAction
{
public:
    SetOutAction();
    AH::GamePhases phases() const override { return AH::Upkeep; }
    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    QString id() const override { return "AC_SETOUT"; }
    QString name() const override { return "Setout"; }
    QString description() const override { return ""; }
};

#endif // SETOUTACTION_H
