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

    static SkipOption *getSkipOption();

protected:
    virtual void enterPhase() {}
    virtual QList<GameAction *> getPhaseActions() { return QList<GameAction* >(); }
    virtual QList<GameOption *> getPhaseOptions() { return QList<GameOption* >(); }
    virtual void finishPhase() {}

    //SkipOption *getSkipOption() { return m_skip; }

    QList<GameOption *> getOptions();
    GameOption *autoChoose(const QList<GameOption *> &options);

protected:
    Game *m_game;

private:
    //SkipOption *m_skip;
    static SkipOption *s_skip;
};

#endif // GAMEPHASE_H
