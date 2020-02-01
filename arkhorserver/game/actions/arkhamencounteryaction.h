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

    QString id() const { return "AC_ARKHAM_ENCOUNTERY"; }

    virtual AH::GamePhases phases() const { return AH::ArkhamEncountery; }

    virtual bool execute();
    bool executeOnPlayer(Player *p, AH::Common::FieldData::FieldID fieldId);
    virtual QString notificationString(NotificationPart part, const QString &desc = QString::null) const;

    virtual QString name() const { return "Arkham Encoutnery"; }
    virtual QString description() const { return ""; }
};

class ArkhamEncounteryOption : public GameOption
{
public:
    ArkhamEncounteryOption()
        : GameOption(&ea, AH::CannotContinue, AH::ChooseOptional, AH::ArkhamEncountery)
    {}

    QString id() const { return "OP_ARKHAM_ENCOUNTERY"; }

    virtual AutoChoose autoChoose() const;
    virtual bool isAvailable() const { return true; }
    virtual AH::Common::ModifiedPropertyValueData baseProperty() const;

private:
    ArkhamEncounteryAction ea;
};

#endif // ARKHAMENCOUNTERYACTION_H
