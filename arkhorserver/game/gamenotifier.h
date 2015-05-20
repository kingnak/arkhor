#ifndef GAMENOTIFIER_H
#define GAMENOTIFIER_H

#include <ahglobal.h>
#include <QList>
#include <QString>

class Player;
class Game;
class GameAction;
class GameOption;
class GameBoard;
class Character;

class GameNotifier
{
public:
    GameNotifier() {}
    virtual ~GameNotifier() {}

    virtual void init(Game *game) = 0;
    virtual void gameStarted() = 0;

    virtual void playerRemoved(Player *p) = 0;

    virtual void startGame() = 0;

    virtual void sendBoard(GameBoard *board) = 0;
    virtual void sendCharacter(Character *c) = 0;

    virtual void nextRound() = 0;
    virtual void gamePhaseChaned(AH::GamePhase phase) = 0;

    virtual void firstPlayerChanged(const Player *player) = 0;
    virtual void currentPlayerChanged(const Player *player) = 0;

    virtual void actionStart(const GameAction *action, QString desc = QString::null) = 0;
    virtual void actionUpdate(const GameAction *action, QString desc = QString::null) = 0;
    virtual void actionFinish(const GameAction *action, QString desc = QString::null) = 0;
    virtual void actionExecute(const GameAction *action, QString desc = QString::null) = 0;
};

#endif // GAMENOTIFIER_H
