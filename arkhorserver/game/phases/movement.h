#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "../gamephase.h"
#include "../gameaction.h"
#include "../actions/moveaction.h"
#include "fight/fightphase.h"

class Movement : public GamePhase
{
public:
    Movement(Game *game);
    ~Movement();

    void characterMoved();

protected:
    QList<GameOption *> getPhaseOptions();
    void enterPhase();
    void finishPhase();

private:
    MoveOption *m_move;
    FightPhase *m_fight;

    FightPhase::FightOutcome m_outcome;
};

#endif // MOVEMENT_H
