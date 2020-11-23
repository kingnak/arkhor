#ifndef CLOSEGATEACTION_H
#define CLOSEGATEACTION_H

#include "../gameaction.h"
#include "../gameoption.h"

class CloseGateAction : public GameAction
{
public:
    CloseGateAction();

    QString id() const override { return "AC_CLOSE_GATE"; }

    AH::GamePhases phases() const override { return AH::ArkhamEncountery; }

    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    QString name() const override;
    QString description() const override { return ""; }

private:
    bool m_bSeal;

    friend class CloseGateOption;
    friend class SealGateOption;
};

class CloseGateOption : public GameOption
{
public:
    CloseGateOption()
        : GameOption(&ca, AH::CannotContinue, AH::ChooseSupplemental, AH::ArkhamEncountery)
    {}

    QString id() const override { return "OP_CLOSE_GATE"; }
    QString sourceId() const override;
    QString name() const override { return "Close Gate"; }

    bool isAvailable() const override;
    bool execute() override;

private:
    CloseGateAction ca;
};

class SealGateOption : public GameOption
{
public:
    SealGateOption()
        : GameOption(&ca, AH::CannotContinue, AH::ChooseSupplemental, AH::ArkhamEncountery)
    {}

    QString name() const override { return "Seal Gate"; }
    QString id() const override { return "OP_SEAL_GATE"; }

    QString sourceId() const override;

    AH::Common::Cost costs() const override;

    bool isAvailable() const override;
    bool execute() override;

private:
    CloseGateAction ca;
};

#endif // CLOSEGATEACTION_H
