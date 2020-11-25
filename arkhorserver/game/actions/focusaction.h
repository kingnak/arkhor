#ifndef FOCUSACTION_H
#define FOCUSACTION_H

#include "../gameaction.h"
#include "../gameoption.h"
#include <QString>

class Player;

class FocusAction : public GameAction
{
public:
    FocusAction();

    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    bool executeOnPlayer(Player *p, int &focusAmount);
    AH::GamePhases phases() const override { return AH::Upkeep; }

    QString id() const override { return "AC_FOCUS"; }
    QString name() const override { return "Focus"; }
    QString description() const override { return "Set the focus attributes"; }
};

class FocusOption : public GameOption
{
public:
    FocusOption()
        : GameOption(&fa, AH::CanContinue, AH::ChooseOptional, AH::Upkeep)
    { }

    QString id() const override { return "OP_FOCUS"; }

    bool isAvailable() const override;

private:
    FocusAction fa;
};

#endif // FOCUSACTION_H
