#ifndef MOVEACTION_H
#define MOVEACTION_H

#include "../gameaction.h"
#include "../gameoption.h"

class Movement;

class MoveAction : public GameAction
{
public:
    MoveAction(Movement *m);

    QString id() const override { return "AC_MOVE"; }

    AH::GamePhases phases() const override { return AH::Movement; }

    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    QString name() const override { return "Move"; }
    QString description() const override { return "Move Character"; }

private:
    bool moveArkham();
    bool moveOtherWorld();

    Movement *m_movement;
};

class MoveOption : public GameOption
{
public:
    MoveOption(Movement *m) : GameOption(&ma, AH::CanContinue, AH::ChooseOptional, AH::Movement), ma(m) {}

    QString id() const override { return "OP_MOVE"; }
    AH::Common::ModifiedPropertyValueData baseProperty() const override;

    void determineMovementType();

    bool isAvailable() const override;

private:
    MoveAction ma;
};

#endif // MOVEACTION_H
