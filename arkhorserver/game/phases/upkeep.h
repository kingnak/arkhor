#ifndef UPKEEP_H
#define UPKEEP_H

#include "../gamephase.h"
#include "../gameaction.h"
#include "../actions/focusaction.h"
#include "../actions/untapaction.h"
#include "../actions/setoutaction.h"
#include "../actions/unlockaction.h"

class Upkeep : public GamePhase
{
public:
    Upkeep(Game *game);
    ~Upkeep();

protected:
    QList<GameAction *> getPhaseActions();
    QList<GameOption *> getPhaseOptions();
    void enterPhase();

private:
    SetOutAction *m_setout;
    UntapAction *m_untap;
    FocusOption *m_focus;
    UnlockAction *m_unlock;
};

#endif // UPKEEP_H
