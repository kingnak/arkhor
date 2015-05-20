#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "../gamephase.h"
#include "../gameaction.h"
#include "../actions/moveaction.h"
#include "../actions/evadeaction.h"
#include "../actions/fightaction.h"

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
    EvadeOption *m_sneak;
    FightOption *m_fight;
};

#endif // MOVEMENT_H
