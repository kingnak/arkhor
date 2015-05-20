#ifndef ARKHAMENCOUTERY_H
#define ARKHAMENCOUTERY_H

#include "../gamephase.h"
#include "../actions/arkhamencounteryaction.h"
#include "../actions/entergateaction.h"
#include "../actions/closegateaction.h"

class ArkhamEncoutery : public GamePhase
{
public:
    ArkhamEncoutery(Game *game);

protected:
    virtual QList<GameOption *> getPhaseOptions();
    virtual void enterPhase();

private:
    ArkhamEncounteryOption *m_enc;
    EnterGateOption *m_enter;
    CloseGateOption *m_close;
    SealGateOption *m_seal;
};

#endif // ARKHAMENCOUTERY_H
