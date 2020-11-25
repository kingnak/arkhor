#ifndef UNTAPACTION_H
#define UNTAPACTION_H

#include "../gameaction.h"

class UntapAction : public GameAction
{
public:
    UntapAction();
    AH::GamePhases phases() const override;
    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    QString id() const override { return "AC_UNTAP"; }
    QString name() const override { return "Untap"; }
    QString description() const override { return ""; }
};

#endif // UNTAPACTION_H
