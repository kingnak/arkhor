#ifndef BROADCASTNOTIFIER_H
#define BROADCASTNOTIFIER_H

#include "game/gamenotifier.h"

class BroadcastNotifier : public GameNotifier
{
public:
    BroadcastNotifier();

    virtual void init(Game *game);

    virtual void gameStarted();
    virtual void startGame();

    virtual void sendBoard(GameBoard *board);

    virtual void playerRemoved(Player *p);

    virtual void nextRound();
    virtual void gamePhaseChaned(AH::GamePhase phase);

    virtual void firstPlayerChanged(const Player *player);
    virtual void currentPlayerChanged(const Player *player);

    virtual void actionStart(const GameAction *action, QString desc = QString::null);
    virtual void actionUpdate(const GameAction *action, QString desc = QString::null);
    virtual void actionFinish(const GameAction *action, QString desc = QString::null);
    virtual void actionExecute(const GameAction *action, QString desc = QString::null);

private:
    Game *m_game;
};

#endif // BROADCASTNOTIFIER_H
