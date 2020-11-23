#ifndef ARKHAMENCOUNTERYACTION_H
#define ARKHAMENCOUNTERYACTION_H

#include "../gameaction.h"
#include "../gameoption.h"
#include <fielddata.h>

class Player;

class ArkhamEncounteryAction : public GameAction
{
public:
    ArkhamEncounteryAction() {}

    QString id() const override { return "AC_ARKHAM_ENCOUNTERY"; }

    AH::GamePhases phases() const override { return AH::ArkhamEncountery; }

    bool execute() override;
    bool executeOnPlayer(Player *p, AH::Common::FieldData::FieldID fieldId);
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    QString name() const override { return "Arkham Encoutnery"; }
    QString description() const override { return ""; }
};

class ArkhamEncounteryOption : public GameOption
{
public:
    ArkhamEncounteryOption()
        : GameOption(&ea, AH::CannotContinue, AH::ChooseOptional, AH::ArkhamEncountery)
    {}

    QString id() const override { return "OP_ARKHAM_ENCOUNTERY"; }

    AutoChoose autoChoose() const override;
    bool isAvailable() const override { return true; }
    AH::Common::ModifiedPropertyValueData baseProperty() const override;

private:
    ArkhamEncounteryAction ea;
};

#endif // ARKHAMENCOUNTERYACTION_H
