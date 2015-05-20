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

    bool execute();

    AH::GamePhases phases() const { return AH::Movement; }

    QString id() const { return "AC_PUTONFIELD"; }
    QString name() const { return "Go to " + m_field->name(); }
    QString description() const { return name(); }

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

    QString id() const { return "OP_PUTONFIELD"; }

    virtual bool isAvailable() const;

    void setField(GameField *f) { pofa.m_field = f; }

private:
    PlaceOnFieldAction pofa;
};

#endif // PLACEONFIELDACTION_H
