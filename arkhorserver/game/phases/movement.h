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

protected:
    QList<GameOption *> getPhaseOptions();
    void enterPhase();

private:
    MoveOption *m_move;
    FightPhase *m_fight;
};

#endif // MOVEMENT_H
