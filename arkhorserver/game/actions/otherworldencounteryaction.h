#ifndef OTHERWORLDENCOUNTERYACTION_H
#define OTHERWORLDENCOUNTERYACTION_H

#include "../gameaction.h"
#include "../gameoption.h"
#include <fielddata.h>

class Player;

class OtherWorldEncounteryAction : public GameAction
{
public:
    OtherWorldEncounteryAction();

    QString id() const { return "AC_OTHERWORLD_ENCOUNTERY"; }

    virtual AH::GamePhases phases() const { return AH::OtherWorldEncountery; }

    virtual bool execute();
    bool executeOnPlayer(Player *p, AH::Common::FieldData::FieldID fieldId);
    virtual QString notificationString(NotificationPart part, const QString &desc = QString::null) const;

    virtual QString name() const { return "Other World Encoutnery"; }
    virtual QString description() const { return ""; }
};

class OtherWorldEncounteryOption : public GameOption
{
public:
    OtherWorldEncounteryOption()
        : GameOption(&ea, AH::CannotContinue, AH::ChooseMandatory, AH::OtherWorldEncountery)
    {}

    QString id() const { return "OP_OTHERWORLD_ENCOUNTERY"; }

    virtual AutoChoose autoChoose() const;
    virtual bool isAvailable() const;
    virtual AH::Common::ModifiedPropertyValueData baseProperty() const;

private:
    OtherWorldEncounteryAction ea;
};

#endif // OTHERWORLDENCOUNTERYACTION_H
