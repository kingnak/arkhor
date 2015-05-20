#ifndef GAMEPHASE_H
#define GAMEPHASE_H

#include "game.h"
#include "gameoption.h"
#include "actions/skipoption.h"

class Game;

class GamePhase
{
public:
    GamePhase(Game *game);
    virtual ~GamePhase();

    virtual void execute();

protected:
    virtual void enterPhase() {}
    virtual QList<GameAction *> getPhaseActions() { return QList<GameAction* >(); }
    virtual QList<GameOption *> getPhaseOptions() { return QList<GameOption* >(); }
    virtual void finishPhase() {}

protected:
    Game *m_game;

    SkipOption *getSkipOption() { return m_skip; }

private:
    SkipOption *m_skip;
};

#endif // GAMEPHASE_H
