#ifndef EVADEACTION_H
#define EVADEACTION_H

#include "game/gameaction.h"
#include "game/gameoption.h"

class FightPhase;

class EvadeAction : public GameAction
{
public:
    EvadeAction(FightPhase *fight);

    QString id() const override { return "AC_EVADE"; }

    AH::GamePhases phases() const override { return AH::Movement; }
    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    //QString name() const override { return "Evade"; }
    QString description() const override { return ""; }

private:
    bool m_hasFailedAttempt;
    FightPhase *m_fight;
    friend class EvadeOption;
};

class EvadeOption : public GameOption
{
public:
    EvadeOption(FightPhase *fight)
        : GameOption(&sa, AH::CanContinue, AH::ChooseMandatory, AH::Movement),
          sa(fight)
    {}
    QString id() const override { return "OP_EVADE"; }

    QString sourceId() const override;

    void reset() { sa.m_hasFailedAttempt = false; }
    void setName(QString name) {
        sa.m_name = name;
    }

    AH::Common::ModifiedPropertyValueData baseProperty() const override;

    bool isAvailable() const override;

private:
    EvadeAction sa;
};

#endif // EVADEACTION_H
