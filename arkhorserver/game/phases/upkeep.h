#ifndef UPKEEP_H
#define UPKEEP_H

#include "../gamephase.h"
#include "../gameaction.h"
#include "../actions/focusaction.h"
#include "../actions/untapaction.h"

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
    UntapAction *m_untap;
    FocusOption *m_focus;
};

#endif // UPKEEP_H
