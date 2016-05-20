#ifndef MOVEACTION_H
#define MOVEACTION_H

#include "../gameaction.h"
#include "../gameoption.h"

class Movement;

class MoveAction : public GameAction
{
public:
    MoveAction(Movement *m);

    QString id() const { return "AC_MOVE"; }

    virtual AH::GamePhases phases() const { return AH::Movement; }

    virtual bool execute();
    virtual QString notificationString(NotificationPart part, const QString &desc = QString::null) const;

    virtual QString name() const { return "Move"; }
    virtual QString description() const { return "Move Character"; }

private:
    bool moveArkham();
    bool moveOtherWorld();

    Movement *m_movement;
};

class MoveOption : public GameOption
{
public:
    MoveOption(Movement *m) : GameOption(&ma, AH::CanContinue, AH::ChooseOptional, AH::Movement), ma(m) {}

    QString id() const { return "OP_MOVE"; }

    void determineMovementType();

    virtual bool isAvailable() const;

private:
    MoveAction ma;
};

#endif // MOVEACTION_H
