#include "broadcastnotifier.h"
#include "game.h"
#include "player.h"
#include <QEventLoop>
#include <QTimerEvent>

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

void BroadcastNotifier::sendSetting(AH::Common::GameSettingData data)
{
    foreach (Player *p, m_game->getPlayers()) {
        p->sendSetting(data);
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

bool BroadcastNotifier::acknowledgeMythos(const MythosCard *m, QObject *observer)
{
    Q_ASSERT(observer == NULL);

    m_openAcks = m_game->getPlayers().toSet();
    foreach (Player *p, m_game->getPlayers()) {
        if (p->acknowledgeMythos(m, this)) {
            m_openAcks.remove(p);
        }
    }

    QSet<Player *> missed;
    if (!m_openAcks.empty()) {
        m_ackTimeoutId = startTimer(90*1000);
        m_ackLoop.exec();
        missed = m_openAcks;
        missed.detach();
    }

    foreach (Player *p, missed) {
        p->abortAcknowledge();
    }

    m_openAcks.clear();
    killTimer(m_ackTimeoutId);
    m_ackTimeoutId = 0;

    return true;
}

void BroadcastNotifier::abortAcknowledge()
{
    Q_ASSERT(false);
}

void BroadcastNotifier::notifyWon(QString msg)
{
    foreach (Player *p, m_game->getPlayers()) {
        p->notifyWon(msg);
    }
}

void BroadcastNotifier::notifyLost(QString msg)
{
    foreach (Player *p, m_game->getPlayers()) {
        p->notifyLost(msg);
    }
}

void BroadcastNotifier::notifyDied(Player *dp)
{
    foreach (Player *p, m_game->getPlayers()) {
        p->notifyDied(dp);
    }
}

bool BroadcastNotifier::event(QEvent *eve)
{
    if (eve->type() == QEvent::Timer) {
        if (dynamic_cast<QTimerEvent *> (eve)->timerId() == m_ackTimeoutId) {
            m_ackLoop.exit(1);
            return true;
        }
    } else if (eve->type() == AcknowledgeEvent::Type()) {
        AcknowledgeEvent *aE = dynamic_cast<AcknowledgeEvent*> (eve);
        m_openAcks.remove(aE->player());
        if (m_openAcks.empty()) {
            m_ackLoop.exit(0);
        }
        return true;
    }
    return false;
}

QEvent::Type AcknowledgeEvent::s_type = static_cast<QEvent::Type> (QEvent::registerEventType());
