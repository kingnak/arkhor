#include "broadcastnotifier.h"
#include "game.h"
#include "player.h"

BroadcastNotifier::BroadcastNotifier()
{
}

void BroadcastNotifier::init(Game *game)
{
    m_game = game;
}

void BroadcastNotifier::gameStarted()
{
    foreach (Player *p, m_game->getPlayers()) {
        p->gameStarted();
    }
}

void BroadcastNotifier::startGame()
{
    foreach (Player *p, m_game->getPlayers()) {
        p->startGame();
    }
}

void BroadcastNotifier::sendBoard(GameBoard *board)
{
    foreach (Player *p, m_game->getPlayers()) {
        p->sendBoard(board);
    }
}

void BroadcastNotifier::sendCharacter(Character *c)
{
    foreach (Player *p, m_game->getPlayers()) {
        p->sendCharacter(c);
    }
}

void BroadcastNotifier::playerRemoved(Player *player)
{
    foreach (Player *p, m_game->getPlayers()) {
        p->playerRemoved(player);
    }
}

void BroadcastNotifier::nextRound()
{
    foreach (Player *p, m_game->getPlayers()) {
        p->nextRound();
    }
}

void BroadcastNotifier::gamePhaseChaned(AH::GamePhase phase)
{
    foreach (Player *p, m_game->getPlayers()) {
        p->gamePhaseChaned(phase);
    }
}

void BroadcastNotifier::firstPlayerChanged(const Player *player)
{
    foreach (Player *p, m_game->getPlayers()) {
        p->firstPlayerChanged(player);
    }
}

void BroadcastNotifier::currentPlayerChanged(const Player *player)
{
    foreach (Player *p, m_game->getPlayers()) {
        p->currentPlayerChanged(player);
    }
}

void BroadcastNotifier::actionStart(const GameAction *action, QString desc)
{
    foreach (Player *p, m_game->getPlayers()) {
        p->actionStart(action, desc);
    }
}

void BroadcastNotifier::actionUpdate(const GameAction *action, QString desc)
{
    foreach (Player *p, m_game->getPlayers()) {
        p->actionUpdate(action, desc);
    }
}

void BroadcastNotifier::actionFinish(const GameAction *action, QString desc)
{
    foreach (Player *p, m_game->getPlayers()) {
        p->actionFinish(action, desc);
    }
}

void BroadcastNotifier::actionExecute(const GameAction *action, QString desc)
{
    foreach (Player *p, m_game->getPlayers()) {
        p->actionExecute(action, desc);
    }
}

void BroadcastNotifier::objectsInvalidated(QStringList ids)
{
    foreach (Player *p, m_game->getPlayers()) {
        p->objectsInvalidated(ids);
    }
}

