#ifndef CLOSEGATEACTION_H
#define CLOSEGATEACTION_H

#include "../gameaction.h"
#include "../gameoption.h"

class CloseGateAction : public GameAction
{
public:
    CloseGateAction();

    QString id() const { return "AC_CLOSE_GATE"; }

    virtual AH::GamePhases phases() const { return AH::ArkhamEncountery; }

    virtual bool execute();

    virtual QString name() const;
    virtual QString description() const { return ""; }

private:
    bool m_bSeal;

    friend class CloseGateOption;
    friend class SealGateOption;
};

class CloseGateOption : public GameOption
{
public:
    CloseGateOption()
        : GameOption(&ca, AH::CannotContinue, AH::ChooseOptional, AH::ArkhamEncountery)
    {}

    QString id() const { return "OP_CLOSE_GATE"; }

    virtual bool execute();

    virtual QString name() const { return "Close Gate"; }

    virtual bool isAvailable();

private:
    CloseGateAction ca;
};

class SealGateOption : public GameOption
{
public:
    SealGateOption()
        : GameOption(&ca, AH::CannotContinue, AH::ChooseOptional, AH::ArkhamEncountery)
    {}

    QString id() const { return "OP_SEAL_GATE"; }

    virtual bool execute();
    AH::Common::Cost getCosts() const;

    virtual bool isAvailable();

private:
    CloseGateAction ca;
};

#endif // CLOSEGATEACTION_H
