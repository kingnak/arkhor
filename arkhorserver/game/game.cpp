#include "game.h"
#include "phases/chooseinvestigator.h"
#include "phases/upkeep.h"
#include "phases/movement.h"
#include "phases/arkhamencoutery.h"
#include "phases/otherworldencountery.h"
#include "gameboard.h"
#include "player.h"
#include "investigator.h"
#include "broadcastnotifier.h"
#include "gameobject.h"
#include "arkhamencounter.h"
#include <QThread>
#include <QDebug>

Game *Game::s_instance = NULL;

Game::Game()
:   m_context(this, NULL, NULL, AH::NoGamePhase),
    m_board(NULL),
    m_nextPlayerId(0),
    m_started(false)
{
    Game::s_instance = this;
    m_notifier = new BroadcastNotifier;
    m_board = new GameBoard();
    m_board->init();
}

Game::~Game()
{
    qDeleteAll(m_phases);
    delete m_board;
}

void Game::invoke(const char *method)
{
    Qt::ConnectionType type = (QThread::currentThread() == this->thread()) ? Qt::DirectConnection : Qt::QueuedConnection;
    QMetaObject::invokeMethod(this, method, type);
}

void Game::init()
{
    m_phases
        << new Upkeep(this)
        << new Movement(this)
        << new ArkhamEncoutery(this)
        << new OtherWorldEncountery(this);
}

void Game::start()
{
    if (m_started) return;
    m_started = true;

    m_notifier->init(this);

    m_notifier->gameStarted();

    // Game preparation:
    initBoard();
    chooseInvestigators();
    cleanupDeactivatedPlayers();
    //chooseAnciantOne();

    //initDecks();
}

void Game::play()
{
    mythos();
    m_notifier->firstPlayerChanged(getFirstPlayer());
    while (true) {
        playRound();
    }
}

Game *Game::instance()
{
    return Game::s_instance;
}

QList<Player *> Game::getPlayers()
{
    QReadLocker l(&m_lock);
    return m_player.values();
}

void Game::registerInvestigator(Investigator *i)
{
    QWriteLocker l(&m_lock);
    m_investigators[i->id()] = i;
}

QList<Investigator *> Game::allInvestigators() const
{
    QReadLocker l(&m_lock);
    return m_investigators.values();
}

void Game::registerAction(GameAction *a)
{
    m_actions.insert(a->id(), a);
}

void Game::registerOption(GameOption *o)
{
    m_options.insert(o->id(), o);
}

void Game::registerObject(GameObject *o, quint32 count)
{
    m_objects.insert(o->id(), o);
    m_objectCounts.insert(o->id(), count);
}

void Game::registerArkhamEnconutry(ArkhamEncounter *a)
{
    m_arkEnc[a->fieldId()] << a;
}

bool Game::resolveDependencies()
{
    bool ok = true;
    // Resolve Actions for Options
    foreach (GameOption *o, m_options.values()) {
        ok &= o->resolveDependencies(this);
    }

    // Resolve Objects:
    foreach (GameObject *o, m_objects.values()) {
        ok &= o->resolveDependencies(this);
    }

    // Resolve Field Options
    foreach (AH::Common::FieldData::FieldID fid, m_fieldOptionMap.keys()) {
        foreach (QString opId, m_fieldOptionMap[fid]) {
            GameOption *op = findOptionById(opId);
            if (op) {
                m_board->field(fid)->addFieldOption(op);
            } else {
                qWarning() << "Cannot resolve option" << opId << "for field" << m_board->field(fid)->name();
                ok = false;
            }
        }
    }

    // Resolve encounteries
    typedef QList<ArkhamEncounter *> AEL;
    foreach (AEL lst, m_arkEnc.values()) {
        foreach (ArkhamEncounter *a, lst) {
            ok &= a->resolveDependencies(this);
        }
    }

    return ok;
}

void Game::registerFieldOption(AH::Common::FieldData::FieldID fId, QString opId)
{
    m_fieldOptionMap[fId] << opId;
}

GameAction *Game::findActionById(const QString &id) const
{
    return m_actions.value(id);
}

GameOption *Game::findOptionById(const QString &id) const
{
    return m_options.value(id);
}

GameObject *Game::findObjectById(const QString &id) const
{
    return m_objects.value(id);
}

Player *Game::getFirstPlayer()
{
    QReadLocker l(&m_lock);
    //return m_firstPlayer.value();
    return m_playerList.first();
}

Player *Game::getCurrentPlayer()
{
    QReadLocker l(&m_lock);
    return m_context.m_player;
}

bool Game::addPlayer(Player *p)
{
    QWriteLocker l(&m_lock);
    if (m_started) return false;

    QString id = QString("PL_%1").arg(++m_nextPlayerId);
    p->setId(id);
    m_player[id] = p;
    m_playerList << p;
    return true;
}

void Game::removePlayer(Player *p)
{
    if (p) {
        p->deactivate();
    }
}

GameContext Game::context()
{
    return m_context;
}

GameBoard *Game::board()
{
    return m_board;
}

GameNotifier *Game::notifier()
{
    return m_notifier;
}

// protected
void Game::initBoard()
{
    foreach (GameField *f, m_board->fields(AH::Common::FieldData::Location)) {
        f->putClue();
    }
}

void Game::chooseInvestigators()
{
    ChooseInvestigator ci(this);
    ci.execute();
}

//private below:

void Game::upkeep()
{
    m_notifier->gamePhaseChaned(AH::Upkeep);
    executePlayerPhase(UpkeepIndex, AH::Upkeep);
}

void Game::movement()
{
    m_notifier->gamePhaseChaned(AH::Movement);
    executePlayerPhase(MovementIndex, AH::Movement);
}

void Game::arkhamEncountery()
{
    m_notifier->gamePhaseChaned(AH::ArkhamEncountery);
    executePlayerPhase(ArkhamEncounteryIndex, AH::ArkhamEncountery);
}

void Game::otherWorldEncountery()
{
    m_notifier->gamePhaseChaned(AH::OtherWorldEncountery);
    executePlayerPhase(OtherWorldEncounteryIndex, AH::OtherWorldEncountery);
}

void Game::mythos()
{
    m_notifier->gamePhaseChaned(AH::Mythos);
    // Not a player phase
    //m_context = GameContext(m_player.first(), AH::Mythos);
    //m_phases[MythosIndex]->execute();
}

void Game::executePlayerPhase(int idx, AH::GamePhase phase)
{
    foreach (Player *p, m_player) {
        m_notifier->currentPlayerChanged(p);
        m_context = GameContext(this, p, NULL, phase);
        m_phases[idx]->execute();
    }
    /*
    PlayerMap::iterator it = m_firstPlayer;
    do {
        Player *p = it.value();
        m_notifier->currentPlayerChanged(p);
        m_context = GameContext(this, p, NULL, phase);
        m_phases[idx]->execute();
        ++it;
        if (it == m_player.end()) {
            it = m_player.begin();
        }
    } while (it != m_firstPlayer);
    */
}

void Game::playRound()
{
    upkeep();
    cleanupDeactivatedPlayers();
    movement();
    cleanupDeactivatedPlayers();
    arkhamEncountery();
    cleanupDeactivatedPlayers();
    otherWorldEncountery();
    cleanupDeactivatedPlayers();
    mythos();
    cleanupDeactivatedPlayers();
    nextRound();
}

void Game::nextRound()
{
    /*
    m_firstPlayer++;
    if (m_firstPlayer == m_player.end()) {
        m_firstPlayer = m_player.begin();
    }
    */
    {
        QWriteLocker l(&m_lock);
        Player *p = m_playerList.takeFirst();
        m_playerList.append(p);
    }

    m_notifier->nextRound();
    m_notifier->firstPlayerChanged(getFirstPlayer());
}

void Game::cleanupDeactivatedPlayers()
{
    QList<Player *> lst = m_player.values();
    foreach (Player *p, lst) {
        if (!p->isActive()) {
            m_player.remove(p->id());
            m_playerList.removeAll(p);
            //p->removedFromGame();
            m_notifier->playerRemoved(p);
        }
    }
}
