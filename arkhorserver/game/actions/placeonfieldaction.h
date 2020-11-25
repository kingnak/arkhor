#ifndef PLACEONFIELDACTION_H
#define PLACEONFIELDACTION_H

#include "../gameaction.h"
#include "../gameoption.h"
#include "game/gamefield.h"

class PlaceOnFieldOption;

class PlaceOnFieldAction : public GameAction
{
public:
    PlaceOnFieldAction();

    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    AH::GamePhases phases() const override { return AH::Movement; }

    QString id() const override { return "AC_PUTONFIELD"; }
    QString name() const override { return "Go to " + m_field->name(); }
    QString description() const override { return name(); }

private:
    GameField *m_field;

    friend class PlaceOnFieldOption;
};

class PlaceOnFieldOption : public GameOption
{
public:
    PlaceOnFieldOption()
        : GameOption(&pofa, AH::CannotContinue, AH::ChooseMandatory, AH::Movement)
    { }

    QString id() const override { return "OP_PUTONFIELD"; }

    bool isAvailable() const override;

    void setField(GameField *f) { pofa.m_field = f; }

private:
    PlaceOnFieldAction pofa;
};

#endif // PLACEONFIELDACTION_H
