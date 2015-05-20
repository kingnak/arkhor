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
#include "character.h"
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
    m_registry = new GameRegistry;
    m_notifier = new BroadcastNotifier;
    m_board = new GameBoard();
    m_board->init();
}

Game::~Game()
{
    qDeleteAll(m_phases);
    delete m_board;
    delete m_registry;
}

void Game::invoke(const char *method)
{
    Qt::ConnectionType type = (QThread::currentThread() == this->thread()) ? Qt::DirectConnection : Qt::QueuedConnection;
    QMetaObject::invokeMethod(this, method, type);
}

void Game::initPhases()
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
    initDecks();
    //initMonsters();

    // This will set up main GUI in clients
    m_notifier->startGame();

    initInvestigators();

    initPhases();
    play();
}

void Game::play()
{
    sendBoard();
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
    //return m_player.values();
    return m_registry->allPlayers();
}

void Game::registerInvestigator(Investigator *i)
{
    QWriteLocker l(&m_lock);
    //m_investigators[i->id()] = i;
    if (!m_registry->registerInvestigator(i)) {
        qCritical() << "Error registering investigator";
    }
}

QList<Investigator *> Game::allInvestigators() const
{
    QReadLocker l(&m_lock);
    //return m_investigators.values();
    return m_registry->allInvestigators();
}

void Game::registerCharacter(Character *c)
{
    //m_characters.insert(c->id(), c);
    if (!m_registry->registerCharacter(c)) {
        qCritical() << "Error registering character";
    }
}

void Game::registerAction(GameAction *a)
{
    //m_actions.insert(a->id(), a);
    if (!m_registry->registerAction(a)) {
        qCritical() << "Error registering action";
    }
}

void Game::registerOption(GameOption *o)
{
    //m_options.insert(o->id(), o);
    if (!m_registry->registerOption(o)) {
        qCritical() << "Error registering option";
    }
}

void Game::registerObject(GameObject *o, quint32 count)
{
    //m_objects.insert(o->id(), o);
    //m_objectCounts.insert(o->id(), count);
    if (!m_registry->registerMultiObject(o, count)) {
        qCritical() << "Error registering object(s)";
    }
}

bool Game::resolveDependencies()
{
    bool ok = m_registry->resolveDependencies();
    if (!ok) return false;
    // Resolve Field Options
    foreach (AH::Common::FieldData::FieldID fid, m_fieldOptionMap.keys()) {
        foreach (QString opId, m_fieldOptionMap[fid]) {
            GameOption *op = m_registry->findOptionById(opId);
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
            ok &= a->resolveDependencies(m_registry);
        }
    }
    return ok;
}

void Game::registerArkhamEnconutry(ArkhamEncounter *a)
{
    m_arkEnc[a->fieldId()] << a;
}

void Game::registerFieldOption(AH::Common::FieldData::FieldID fId, QString opId)
{
    m_fieldOptionMap[fId] << opId;
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
    //m_player[id] = p;
    m_registry->registerPlayer(p);
    m_playerList << p;
    return true;
}

void Game::removePlayer(Player *p)
{
    if (p) {
        p->deactivate();
    }
}

void Game::returnMonster(Monster *m)
{
    m_monsterPool.addCard(m);
}

GameContext &Game::context()
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

void Game::boardDirty()
{
    // TODO: better...
    sendBoard();
}

void Game::characterDirty(Character *c)
{
    Player *p = playerForCharacter(c);
    if (p) {
        p->sendCharacter(c);
    }
}

Player *Game::playerForCharacter(Character *c)
{
    foreach (Player *p, m_playerList) {
        if (p->getCharacter() == c) {
            return p;
        }
    }

    return NULL;
}

AH::Common::DescribeObjectsData Game::describeObjects(const AH::Common::RequestObjectsData &reqs) const
{
    AH::Common::DescribeObjectsData ret;
    foreach (AH::Common::RequestObjectsData::ObjectRequest r, reqs.getRequests()) {
        AH::Common::DescribeObjectsData::ObjectDescription d;
        d.first = AH::Common::RequestObjectsData::Unknown;

        switch (r.first) {
        case AH::Common::RequestObjectsData::Unknown:
        {
            GameObject *obj = m_registry->findObjectById(r.second);
            Character *c = m_registry->findCharacterById(r.second);
            if (obj) {
                d.second << *(obj->data());
                d.first = AH::Common::RequestObjectsData::Object;
            } else if (c) {
                d.second << *(c->data());
                d.first = AH::Common::RequestObjectsData::Character;
            }
            // TODO: Extend
            break;
        }
        case AH::Common::RequestObjectsData::Object:
        {
            GameObject *obj = m_registry->findObjectById(r.second);
            if (obj) {
                d.second << *(obj->data());
                d.first = AH::Common::RequestObjectsData::Object;
            }
            break;
        }
        case AH::Common::RequestObjectsData::Monster:
            break;
        case AH::Common::RequestObjectsData::Character:
        {
            Character *c = m_registry->findCharacterById(r.second);
            if (c) {
                d.second << *(c->data());
                d.first = AH::Common::RequestObjectsData::Character;
            }
            break;
        }
        case AH::Common::RequestObjectsData::Gate:
            break;
        //case AH::Common::RequestObjectsData::Board:
        //  break;
        }

        ret.addDescription(d);
    }

    return ret;
}

// protected
// TEST
#include "gate.h"
void Game::initBoard()
{
    foreach (GameField *f, m_board->fields(AH::Common::FieldData::Location)) {
        f->putClue();
    }

    // TEST
    m_board->field(AH::Common::FieldData::DT_ArkhamAsylum)->setGate(new Gate(AH::Dim_Bar, -2, m_board->field(AH::Common::FieldData::OW_Abyss)));
}

void Game::initDecks()
{
    foreach (GameObject *o, m_registry->allObjects()) {
        m_objectDecks[o->type()].addCard(o);
    }

    foreach (AH::GameObjectType t, m_objectDecks.keys()) {
        m_objectDecks[t].shuffle();
    }
}

void Game::chooseInvestigators()
{
    ChooseInvestigator ci(this);
    ci.execute();
}

void Game::initInvestigators()
{
    // FIXED POSSESSION
    foreach (Character *c, m_registry->allCharacters())
    {
        foreach (QString tid, c->investigator()->fixedPossesionObjectIds())
        {
            GameObject *obj = NULL;
            const GameObject *proto = m_registry->findObjectPrototypeByType(tid);
            if (proto) {
                obj = m_objectDecks[proto->type()].drawSpecificByTypeId(tid);
                if (obj) {
                    c->inventory().append(obj);
                } else {
                    qWarning() << "Cannot resolve fixed possession: No more objects of type" << tid;
                }
            } else {
                qCritical() << "Cannot resolve fixed possession: Object type" << tid << "not registered";
            }
        }
    }

    // RANDOM POSSESSION
    foreach (Character *c, m_registry->allCharacters())
    {
        foreach (AH::Common::InvestigatorData::ObjectTypeCount otc, c->investigator()->randomPossesions()) {
            // TODO: MUST BE INTERACTION! IF THERE IS A SPECIAL ABILITY BY INVESTIGATOR,
            // HE MIGHT CHOOSE FROM VARIOUS CARDS!
            for (int i = 0; i < otc.amount; ++i) {
                GameObject *obj = m_objectDecks[otc.type].draw();
                if (obj)
                    c->inventory().append(obj);
                else
                    qWarning() << "No more objects available for random possessions";
            }
        }
    }

    // START FIELD
    foreach (Character *c, m_registry->allCharacters())
    {
        m_board->field(c->investigator()->startFieldId())->placeCharacter(c);
    }
    sendBoard();

    // INITIAL FOCUS
    foreach (Player *p, m_registry->allPlayers())
    {
        FocusAction fa;
        int amount = 100;
        fa.executeOnPlayer(p, amount);
    }
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
    foreach (Player *p, m_registry->allPlayers()) {
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
    //QList<Player *> lst = m_player.values();
    QList<Player *> lst = m_registry->allPlayers();
    foreach (Player *p, lst) {
        if (!p->isActive()) {
            //m_player.remove(p->id());
            m_registry->removePlayer(p);
            m_playerList.removeAll(p);
            //p->removedFromGame();
            m_notifier->playerRemoved(p);
        }
    }
}

void Game::sendBoard()
{
    // TODO: Check if board is dirty?
    m_notifier->sendBoard(m_board);
}
