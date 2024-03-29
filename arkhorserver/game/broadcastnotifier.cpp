#include "broadcastnotifier.h"
#include "game.h"
#include "player.h"
#include <QEventLoop>
#include <QTimerEvent>

BroadcastNotifier::BroadcastNotifier()
:   m_game(nullptr),
    m_ackTimeoutId(0)
{
}

void BroadcastNotifier::init(Game *game)
{
    m_game = game;
}

void BroadcastNotifier::gameStarted()
{
    for (auto p : m_game->getPlayers()) {
        p->gameStarted();
    }
}

void BroadcastNotifier::startGame()
{
    for (auto p : m_game->getPlayers()) {
        p->startGame();
    }
}

void BroadcastNotifier::sendBoard(GameBoard *board, AH::Common::GameBoardChangeData changes)
{
    for (auto p : m_game->getPlayers()) {
        p->sendBoard(board, changes);
    }
}

void BroadcastNotifier::sendBoardDescription(GameBoard *board, QVariantMap descriptions)
{
    for (Player *p : m_game->getPlayers())
        p->sendBoardDescription(board, descriptions);
}

void BroadcastNotifier::sendCharacter(Character *c)
{
    for (auto p : m_game->getPlayers()) {
        p->sendCharacter(c);
    }
}

void BroadcastNotifier::sendSetting(AH::Common::GameSettingData data)
{
    for (auto p : m_game->getPlayers()) {
        p->sendSetting(data);
    }
}

void BroadcastNotifier::playerRemoved(Player *player)
{
    for (auto p : m_game->getPlayers()) {
        p->playerRemoved(player);
    }
}

void BroadcastNotifier::nextRound()
{
    for (auto p : m_game->getPlayers()) {
        p->nextRound();
    }
}

void BroadcastNotifier::gamePhaseChanged(AH::GamePhase phase)
{
    for (auto p : m_game->getPlayers()) {
        p->gamePhaseChanged(phase);
    }
}

void BroadcastNotifier::firstPlayerChanged(const Player *player)
{
    for (auto p : m_game->getPlayers()) {
        p->firstPlayerChanged(player);
    }
}

void BroadcastNotifier::currentPlayerChanged(const Player *player)
{
    for (auto p : m_game->getPlayers()) {
        p->currentPlayerChanged(player);
    }
}

void BroadcastNotifier::actionStart(const GameAction *action, QString desc)
{
    for (auto p : m_game->getPlayers()) {
        p->actionStart(action, desc);
    }
}

void BroadcastNotifier::actionUpdate(const GameAction *action, QString desc)
{
    for (auto p : m_game->getPlayers()) {
        p->actionUpdate(action, desc);
    }
}

void BroadcastNotifier::actionFinish(const GameAction *action, QString desc)
{
    for (auto p : m_game->getPlayers()) {
        p->actionFinish(action, desc);
    }
}

void BroadcastNotifier::actionExecute(const GameAction *action, QString desc)
{
    for (auto p : m_game->getPlayers()) {
        p->actionExecute(action, desc);
    }
}

void BroadcastNotifier::notifySimple(const QString &str, Player *p, const QString &desc)
{
    for (auto c : m_game->getPlayers()) {
        c->notifySimple(str, p, desc);
    }
}

void BroadcastNotifier::notifySpecific(const QString &strThat, const QString &strOther, Player *that, const QString &desc)
{
    for (auto p : m_game->getPlayers()) {
        p->notifySpecific(strThat, strOther, that, desc);
    }
}

void BroadcastNotifier::objectsInvalidated(QStringList ids)
{
    for (auto p : m_game->getPlayers()) {
        p->objectsInvalidated(ids);
    }
}

void BroadcastNotifier::objectTypeInvalidated(AH::Common::RequestObjectsData::ObjectType type)
{
    for (auto p : m_game->getPlayers()) {
        p->objectTypeInvalidated(type);
    }
}

void BroadcastNotifier::setTempData(const QString &data)
{
    for (auto p : m_game->getPlayers()) {
        p->setTempData(data);
    }
}

void BroadcastNotifier::clearTempData()
{
    for (auto p : m_game->getPlayers()) {
        p->clearTempData();
    }
}

bool BroadcastNotifier::acknowledgeMythos(const MythosCard *m, QObject *observer)
{
    Q_ASSERT(observer == nullptr);
    return doAcknowledge([=](GameNotifier *n) {
        return n->acknowledgeMythos(m, this);
    });
}

bool BroadcastNotifier::acknowledgeMonsterMovement(Monster *m, QObject *observer)
{
    Q_ASSERT(observer == nullptr);
    return doAcknowledge([=](GameNotifier *n) {
        return n->acknowledgeMonsterMovement(m, this);
    });
}

bool BroadcastNotifier::doAcknowledge(std::function<bool(GameNotifier *)> func)
{
    m_openAcks = m_game->getPlayers().toSet();
    for (auto p : m_game->getPlayers()) {
        if (func(p)) {
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

    for (auto p : missed) {
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
    for (auto p : m_game->getPlayers()) {
        p->notifyWon(msg);
    }
}

void BroadcastNotifier::notifyLost(QString msg)
{
    for (auto p : m_game->getPlayers()) {
        p->notifyLost(msg);
    }
}

void BroadcastNotifier::notifyDied(Player *dp)
{
    for (auto p : m_game->getPlayers()) {
        p->notifyDied(dp);
    }
}

void BroadcastNotifier::notifyAlert(const QString &msg, Player *p, const QString &desc)
{
    for (auto c : m_game->getPlayers()) {
        c->notifyAlert(msg, p, desc);
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

void BroadcastNotifier::flush()
{
    for (auto c : m_game->getPlayers()) {
        c->flush();
    }
}

const QEvent::Type AcknowledgeEvent::s_type = static_cast<QEvent::Type> (QEvent::registerEventType());
