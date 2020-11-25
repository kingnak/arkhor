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

    QString id() const override { return "AC_OTHERWORLD_ENCOUNTERY"; }

    AH::GamePhases phases() const override { return AH::OtherWorldEncountery; }

    bool execute() override;
    bool executeOnPlayer(Player *p, AH::Common::FieldData::FieldID fieldId);
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    QString name() const override { return "Other World Encoutnery"; }
    QString description() const override { return ""; }
};

class OtherWorldEncounteryOption : public GameOption
{
public:
    OtherWorldEncounteryOption()
        : GameOption(&ea, AH::CannotContinue, AH::ChooseMandatory, AH::OtherWorldEncountery)
    {}

    QString id() const override { return "OP_OTHERWORLD_ENCOUNTERY"; }

    AutoChoose autoChoose() const override;
    bool isAvailable() const override;
    AH::Common::ModifiedPropertyValueData baseProperty() const override;

private:
    OtherWorldEncounteryAction ea;
};

#endif // OTHERWORLDENCOUNTERYACTION_H
