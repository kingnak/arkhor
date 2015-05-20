#include "game.h"
#include "phases/chooseinvestigator.h"
#include "phases/upkeep.h"
#include "phases/movement.h"
#include "phases/arkhamencoutery.h"
#include "phases/otherworldencountery.h"
#include "phases/mythos.h"
#include "gameboard.h"
#include "player.h"
#include "investigator.h"
#include "broadcastnotifier.h"
#include "gameobject.h"
#include "arkhamencounter.h"
#include "otherworldencounter.h"
#include "character.h"
#include "monster.h"
#include "gate.h"
#include "mythoscard.h"
#include <QThread>
#include <QDebug>

Game *Game::s_instance = NULL;

Game::Game()
:   m_context(this, NULL, NULL, AH::NoGamePhase),
    m_board(NULL),
    m_nextPlayerId(0),
    m_started(false),
    m_terrorLevel(1)
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
        << new OtherWorldEncountery(this)
        << new Mythos(this);
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
    initMonsters();

    // This will set up main GUI in clients
    m_notifier->startGame();

    initInvestigators();

    initPhases();
    play();
}

void Game::play()
{
    commitUpdates();
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
    if (count == 0) {
        if (!m_registry->registerSingleObject(o)) {
            qCritical() << "Error registering single object";
        }
    } else if (!m_registry->registerMultiObject(o, count)) {
        qCritical() << "Error registering object(s)";
    }
}

bool Game::resolveDependencies()
{
    m_registry->registerOption(GamePhase::getSkipOption());

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
    /*
    typedef QList<ArkhamEncounter *> AEL;
    foreach (AEL lst, m_arkEnc.values()) {
        foreach (ArkhamEncounter *a, lst) {
            ok &= a->resolveDependencies(m_registry);
        }
    }

    foreach (OtherWorldEncounter *e, m_owEnc) {
        ok &= e->resolveDependencies(m_registry);
    }
    */

    return ok;
}

void Game::registerArkhamEnconutry(ArkhamEncounter *a)
{
    //m_arkEnc[a->fieldId()] << a;
    if (!m_registry->registerArkhamEncounter(a)) {
        qCritical() << "Error registering Arkham Encounter";
    }
}

void Game::registerOtherWorldEncountery(OtherWorldEncounter *e)
{
    //m_owEnc << e;
    if (!m_registry->registerOtherWorldEncounter(e)) {
        qCritical() << "Error registering Other World Encounter";
    }
}

void Game::registerMonster(Monster *m, quint32 count)
{
    if (!m_registry->registerMonster(m, count)) {
        qCritical() << "Error registering monster(s)";
    }
}

void Game::registerMythos(MythosCard *m)
{
    if (!m_registry->registerMythosCard(m)) {
        qCritical() << "Error registering mythos card";
    }
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
    if (!m) return;
    if (m->field()) m->field()->removeMonster(m);
    m_monsterPool.returnToDeck(m);
}

Monster *Game::drawMonster()
{
    m_monsterPool.shuffle();
    return m_monsterPool.draw();
}

MythosCard *Game::drawMythos()
{
    MythosCard *m = m_mythosDeck.draw();
    m->resolveDynamicAttributes();
    return m;
}

void Game::returnMythos(MythosCard *m)
{
    m_mythosDeck.returnToDeck(m);
}

bool Game::createGate(GameField *field)
{
    if (field->isSealed()) {
        return false;
    }

    if (field->gate() != NULL) {
        // Create Monsters: max of gates and players
        int playerCount = getPlayers().count();
        int gateCount = m_board->getGates().count();
        int monsterCount = qMax(playerCount, gateCount);
        QList<Gate *> gates = m_board->getGates();

        // Distribute over gate fields, start the current field
        int idx = gates.indexOf(field->gate());
        if (idx < 0) {
            idx = 0;
        }
        for (int i = 0; i < monsterCount; ++i) {
            GameField *f = gates[idx]->sourceField();
            createMonster(f);
            idx++;
            idx %= gates.count();
        }

        return false;
    } else {
        // TODO: place doom token

        // Create gate
        int adj = RandomSource::instance().nextUint(0, 4)-2;
        int dest = RandomSource::instance().nextUint(0, 7);
        AH::Dimension d = randomDimension();
        AH::Common::FieldData::FieldID fid = static_cast<AH::Common::FieldData::FieldID> (0x1000 + 0x0100*dest);
        GameField *f = m_board->field(fid);
        Gate *g = new Gate(d, adj, f);
        m_registry->registerGate(g);
        field->setGate(g);

        // remove clues
        field->takeClues();

        // Draw investigators
        QList<Character *> characters = field->characters();
        foreach (Character *c, characters) {
            c->setDelayed(true);
            g->enter(c);
        }

        // Create monsters
        int monsterCount = context().getGameProperty(PropertyValue::Game_MonsterCountFromGates).finalVal();
        for (int i = 0; i < monsterCount; ++i) {
            createMonster(field);
        }
        return true;
    }
}

bool Game::createMonster(GameField *field)
{
    Monster *m = drawMonster();
    if (!m) {
        return false;
    }

    if (m->dimension() == AH::NoDimension) {
        // Set random dimension
        m->setDimension(randomDimension());
    }

    int curMonsterCount = m_board->getBoardMonsters().count();
    int maxMonsterCount = context().getGameProperty(PropertyValue::Game_MaxBoardMonsterCount).finalVal();
    if (curMonsterCount >= maxMonsterCount) {
        // Should be at most max!
        // put on outskirts
        putOutskirtsMonster(m);
        return false;
    }

    field->placeMonster(m);
    return true;
}

bool Game::putOutskirtsMonster(Monster *m)
{
    QList<Monster *> outskirtsMonsters = m_board->field(AH::Common::FieldData::Sp_Outskirts)->monsters();
    int outskirtCount = outskirtsMonsters.count();
    int maxOutskritsMonsters = context().getGameProperty(PropertyValue::Game_MaxOutskirtsMonsterCount).finalVal();

    if (outskirtCount >= maxOutskritsMonsters) {
        // remove from outskirts and increase terror track
        returnMonster(m);
        foreach (Monster *mm, outskirtsMonsters) {
            returnMonster(mm);
        }

        increaseTerrorLevel();
        return false;
    }

    m_board->field(AH::Common::FieldData::Sp_Outskirts)->placeMonster(m);
    return true;
}

void Game::closeGate(Gate *g, Character *c)
{
    g->close(c);
    // Return monsters
    QList<Monster *> monsters = m_board->getBoardMonsters();
    monsters << m_board->field(AH::Common::FieldData::Sp_Outskirts)->monsters();

    AH::Dimensions closeDim = g->dimensions();
    foreach (Monster *m, monsters) {
        if (closeDim.testFlag(m->dimension())) {
            returnMonster(m);
        }
    }
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

ArkhamEncounter *Game::drawArkhamEncounter(AH::Common::FieldData::FieldID field)
{
    m_arkEncDecks[field].shuffle();
    ArkhamEncounter *enc = m_arkEncDecks[field].draw();
    if (enc) {
        m_arkEncDecks[field].returnToDeck(enc);
    }
    return enc;
}

OtherWorldEncounter *Game::drawOtherWorldEncounter(AH::Common::FieldData::FieldID field)
{
    AH::OtherWorldColors colors = m_board->colorsForOtherWorld(field);
    Q_ASSERT_X(colors != AH::OWC_NoColor, "Game::drawOtherWorldEncounter", "Field has no colors. Is it an other world field?");

    // Sanity check...
    int ct = m_owEncDeck.size();

    // Draw until matching color is found
    OtherWorldEncounter *e = NULL;
    do {
        ct--;
        e = m_owEncDeck.draw();
        m_owEncDeck.returnToDeck(e);
        // TEST: Allow multi colored cards
        //if (colors.testFlag(e->color())) {
        if ((colors & e->color()) != 0) {
            // Check if field matches (no field matches all)
            if (e->fieldId() != AH::Common::FieldData::NO_NO_FIELD) {
                if (e->fieldId() != field) {
                    e = NULL;
                }
            }
        } else {
            e = NULL;
        }
    } while (!e && ct >= 0);

    if (!e) {
        qWarning() << "Could not find an OW Encountery for field" << field;
    }

    return e;
}

void Game::boardDirty()
{
    m_board->setDirty();
}

void Game::characterDirty(Character *c)
{
    c->setDirty(true);
}

void Game::invalidateObject(QString id)
{
    m_invalidatedObjects << id;
}

void Game::commitUpdates()
{
    if (m_board->isDirty()) {
        m_board->setDirty(false);
        sendBoard();
    }

    foreach (Character *c, m_registry->allCharacters()) {
        if (c->isDirty()) {
            c->setDirty(false);
            Player *p = playerForCharacter(c);
            if (p) {
                p->sendCharacter(c);
            }
        }
    }

    if (!m_invalidatedObjects.isEmpty()) {
        QStringList lst = QStringList(m_invalidatedObjects.toList());
        m_notifier->objectsInvalidated(lst);

        m_invalidatedObjects.clear();
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
        ret.addDescription(describeObject(r));
    }
    return ret;
}

AH::Common::DescribeObjectsData::ObjectDescription Game::describeObject(const AH::Common::RequestObjectsData::ObjectRequest &r) const
{
    AH::Common::DescribeObjectsData::ObjectDescription d;
    d.type = AH::Common::RequestObjectsData::Unknown;

    switch (r.first) {
    case AH::Common::RequestObjectsData::Unknown:
    {
        GameObject *obj = m_registry->findObjectById(r.second);
        if (obj) return describeObject(qMakePair(AH::Common::RequestObjectsData::Object, r.second));

        Character *c = m_registry->findCharacterById(r.second);
        if (c) return describeObject(qMakePair(AH::Common::RequestObjectsData::Character, r.second));

        Monster *m = m_registry->findMonsterById(r.second);
        if (m) return describeObject(qMakePair(AH::Common::RequestObjectsData::Monster, r.second));

        Gate *g = m_registry->findGateById(r.second);
        if (g) return describeObject(qMakePair(AH::Common::RequestObjectsData::Gate, r.second));

        // TODO: Extend
        break;
    }
    case AH::Common::RequestObjectsData::Object:
    {
        GameObject *obj = m_registry->findObjectById(r.second);
        if (obj) {
            d.data << *(obj->data());
            d.id = obj->id();
            d.type = AH::Common::RequestObjectsData::Object;
        }
        break;
    }
    case AH::Common::RequestObjectsData::Monster:
    {
        Monster *m = m_registry->findMonsterById(r.second);
        if (m) {
            d.data << *(m->data());
            d.id = m->id();
            d.type = AH::Common::RequestObjectsData::Monster;
        }
        break;
    }
    case AH::Common::RequestObjectsData::Character:
    {
        Character *c = m_registry->findCharacterById(r.second);
        if (c) {
            d.data << *(c->data());
            d.id = c->id();
            d.type = AH::Common::RequestObjectsData::Character;
        }
        break;
    }
    case AH::Common::RequestObjectsData::Gate:
    {
        Gate *g = m_registry->findGateById(r.second);
        if (g) {
            d.data << *(g->data());
            d.id = g->id();
            d.type = AH::Common::RequestObjectsData::Gate;
        }
        break;
    }
    //case AH::Common::RequestObjectsData::Board:
    //  break;
    }

    return d;
}

AH::Dimension Game::randomDimension() const
{
    int dim = RandomSource::instance().nextUint(0, 9);
    return static_cast<AH::Dimension> (1<<dim);
}

AH::Common::FieldData::FieldID Game::randomLocation(bool onlyStable) const
{
    int district = RandomSource::instance().nextUint(1, 9) * 0x0100;
    int fld = RandomSource::instance().nextUint(1,3);
    int id = district | fld;
    if (id == 0x0703) {
        return randomLocation(onlyStable);
    }

    AH::Common::FieldData::FieldID ret = static_cast<AH::Common::FieldData::FieldID> (id);
    Q_ASSERT(m_board->field(ret) != NULL);
    Q_ASSERT(m_board->field(ret)->type() == AH::Common::FieldData::Location);
    return ret;
}

// protected

void Game::initBoard()
{
    foreach (GameField *f, m_board->fields(AH::Common::FieldData::Location)) {
        f->putClue();
    }

    // TEST
    //m_board->field(AH::Common::FieldData::DT_ArkhamAsylum)->setGate(new Gate(AH::Dim_Slash, -2, m_board->field(AH::Common::FieldData::OW_Abyss)));
    //m_board->field(AH::Common::FieldData::DT_IndependenceSquare)->setGate(new Gate(AH::Dim_Circle, +2, m_board->field(AH::Common::FieldData::OW_Abyss)));
}

void Game::initDecks()
{
    foreach (GameObject *o, m_registry->allObjects()) {
        m_objectDecks[o->type()].addCard(o);
    }
    foreach (AH::GameObjectType t, m_objectDecks.keys()) {
        m_objectDecks[t].shuffle();
    }

    // Shuffeled at each draw, so no need here
    foreach (ArkhamEncounter *ae, m_registry->allArkhamEncounters()) {
        m_arkEncDecks[ae->fieldId()].addCard(ae);
    }

    foreach (OtherWorldEncounter *e, m_registry->allOtherWorldEncounters()) {
        m_owEncDeck.addCard(e);
    }
    m_owEncDeck.shuffle();

    foreach (MythosCard *m, m_registry->allMythosCards()) {
        m_mythosDeck.addCard(m);
    }
    m_mythosDeck.shuffle();
}

void Game::initMonsters()
{
    foreach (Monster *m, m_registry->allMonsters())
        m_monsterPool.addCard(m);

    // Shuffeled at each draw, no need here

    // TEST
    /*
    for (int i = 0;i < 4; ++i)
        m_board->field(AH::Common::FieldData::DT_Downtown)->placeMonster(drawMonster());
    for (int i = 0;i < 6; ++i) {
        Monster *m = drawMonster();
        if (m->movementType() == AH::Common::MonsterData::Flying)
            m_board->field(AH::Common::FieldData::Sp_Sky)->placeMonster(m);
        else
            m_board->field(AH::Common::FieldData::Sp_Outskirts)->placeMonster(m);
    }
    */
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
        // Will be dirty after all of this...
        c->setDirty();
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

    commitUpdates();

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
    m_context = GameContext(this, getFirstPlayer(), NULL, AH::Mythos);
    m_phases[MythosIndex]->execute();
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
    commitUpdates();

    movement();
    cleanupDeactivatedPlayers();
    commitUpdates();

    arkhamEncountery();
    cleanupDeactivatedPlayers();
    commitUpdates();

    otherWorldEncountery();
    cleanupDeactivatedPlayers();
    commitUpdates();

    mythos();
    cleanupDeactivatedPlayers();
    commitUpdates();

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
    commitUpdates();
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
    m_notifier->sendBoard(m_board);
}
