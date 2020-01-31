#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "../gamephase.h"
#include "../gameaction.h"
#include "../actions/moveaction.h"
#include "../actions/undelayaction.h"
#include "../actions/placeonfieldaction.h"
#include "../actions/tradeaction.h"
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
    UndelayOption *m_undelay;
    MoveOption *m_move;
    PlaceOnFieldOption *m_place;
    TradeOption *m_trade;
    FightPhase *m_fight;

    FightPhase::FightOutcome m_outcome;
};

#endif // MOVEMENT_H
