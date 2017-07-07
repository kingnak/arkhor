#include "game.h"
#include "phases/chooseinvestigator.h"
#include "phases/upkeep.h"
#include "phases/movement.h"
#include "phases/arkhamencoutery.h"
#include "phases/otherworldencountery.h"
#include "phases/mythos.h"
#include "phases/ancientoneattack.h"
#include "phases/attackancientonephase.h"
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
#include "ancientone.h"
#include "gamesettingdata.h"
#include "actions/preventdamageaction.h"
#include <QThread>
#include <QDebug>

#ifdef TEST_SCRIPT_BUILD
#include "scripttest/scripttestconfigwidget.h"
#include "scripttest/scripttestconfig.h"
#include "scripttest/scripttestdrawwidget.h"
#include <functional>

ScriptTestConfigWidget *scriptTestConfigWgt = nullptr;

template<typename T, typename W = ScriptTestDrawWidget>
T *scriptTestDrawHelper(const QString &title, Deck<T> &d, std::function<bool()> check = nullptr, bool drawRandom = true, QString *pMore = nullptr)
{
    T *ret = nullptr;
    if (!check || check()) {
        QStringList lst;
        for (auto o : d.all()) {
            QString id = o->id();
            if (!id.isEmpty()) lst << id;
        }
        if (!lst.isEmpty()) {
			W wgt(scriptTestConfigWgt);
            QString id = wgt.askDraw(title, lst);
            if (!id.isNull()) ret = d.drawSpecificById(id);
			if (pMore) *pMore = wgt.moreData();
        }
    }
    if (!ret && drawRandom) {
        ret = d.draw();
    }
    return ret;
}

#endif

Game *Game::s_instance = NULL;

Game::Game()
:   m_context(this, NULL, NULL, AH::NoGamePhase),
    m_board(NULL),
    m_environment(NULL),
    m_rumor(NULL),
    m_ancientOne(NULL),
    m_nextPlayerId(0),
    m_started(false),
    m_settingDirty(false),
    m_terrorLevel(0),
    m_nextSpecialActionNr(1)
{
    Game::s_instance = this;
    m_registry = new GameRegistry;
    m_notifier = new BroadcastNotifier;
    m_board = new GameBoard();
    m_board->init();

#ifdef TEST_SCRIPT_BUILD
    scriptTestConfigWgt = new ScriptTestConfigWidget;
    scriptTestConfigWgt->show();
#endif
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
        << new Mythos(this)
        << new AttackAncientOnePhase(this)
        << new AncientOneAttackPhase(this);
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
    chooseAncientOne();
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
    // TEST
    /*
    Player *p = getFirstPlayer();
    for (int i = 0; i < 10; ++i) {
        p->getCharacter()->addMonsterTrophy(drawMonster());
    }
    p->getCharacter()->addGateTrophy(new Gate(AH::Dim_Circle, 0, m_board->field(AH::Common::FieldData::OW_Abyss)));
    //p->getCharacter()->addGateTrophy(new Gate(AH::Dim_Circle, 0, m_board->field(AH::Common::FieldData::OW_Abyss)));
    */

    commitUpdates();
    mythos();
    m_notifier->firstPlayerChanged(getFirstPlayer());
    bool cont = true;
    while (cont) {
        cont = playRound();
    }

    GameState gs = checkGameState();
    switch (gs) {
    case GS_Win_ClosedGates:
    case GS_Win_DefeatedAncientOne:
    case GS_Win_SealedGates:
        won(gs);
        break;
    case GS_Lost:
        lost(gs);
        break;
    case GS_AwakeAncientOne:
        awakeAncientOne();
        break;
    case GS_Continue:
        Q_ASSERT_X(false, "Game::play()", "Should not be in continue state after main game");
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

bool Game::registerInvestigator(Investigator *i)
{
    QWriteLocker l(&m_lock);
    //m_investigators[i->id()] = i;
    if (!m_registry->registerInvestigator(i)) {
        qCritical() << "Error registering investigator";
        return false;
    }
    m_investigators.addCard(i);
    return true;
}

QList<Investigator *> Game::allInvestigators() const
{
    QReadLocker l(&m_lock);
    //return m_investigators.values();
    return m_registry->allInvestigators();
}

Investigator *Game::drawInvestigator()
{
    m_investigators.shuffle();
    return m_investigators.draw();
}

Investigator *Game::drawSpecificInvestigator(QString id)
{
    return m_investigators.drawSpecificById(id);
}

void Game::returnInvestigator(Investigator *i)
{
    if (i) i->reset();
    m_investigators.returnToDeck(i);
}

bool Game::registerCharacter(Character *c)
{
    //m_characters.insert(c->id(), c);
    if (!m_registry->registerCharacter(c)) {
        qCritical() << "Error registering character";
        return false;
    }
    return true;
}

bool Game::registerAction(GameAction *a)
{
    //m_actions.insert(a->id(), a);
    if (!m_registry->registerAction(a)) {
        qCritical() << "Error registering action";
        return false;
    }
    return true;
}

bool Game::registerOption(GameOption *o)
{
    //m_options.insert(o->id(), o);
    if (!m_registry->registerOption(o)) {
        qCritical() << "Error registering option";
        return false;
    }
    return true;
}

bool Game::registerObject(GameObject *o, qint32 count)
{
    if (count == 0) {
        if (!m_registry->registerSingleObject(o)) {
            qCritical() << "Error registering single object";
            return false;
        }
    } else if (!m_registry->registerMultiObject(o, count)) {
        qCritical() << "Error registering object(s)";
        return false;
    }
    return true;
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

bool Game::registerArkhamEnconutry(ArkhamEncounter *a)
{
    //m_arkEnc[a->fieldId()] << a;
    if (!m_registry->registerArkhamEncounter(a)) {
        qCritical() << "Error registering Arkham Encounter";
        return false;
    }
    return true;
}

bool Game::registerOtherWorldEncountery(OtherWorldEncounter *e)
{
    //m_owEnc << e;
    if (!m_registry->registerOtherWorldEncounter(e)) {
        qCritical() << "Error registering Other World Encounter";
        return false;
    }
    return true;
}

bool Game::registerMonster(Monster *m, quint32 count)
{
    if (!m_registry->registerMonster(m, count)) {
        qCritical() << "Error registering monster(s)";
        return false;
    }
    return true;
}

bool Game::registerMythos(MythosCard *m)
{
    //if (m->id() != "MY_TEST") return true;
    if (!m_registry->registerMythosCard(m)) {
        qCritical() << "Error registering mythos card";
        return false;
    }
    return true;
}

bool Game::registerAncientOne(AncientOne *ao)
{
    if (!m_registry->registerAncientOne(ao)) {
        qCritical() << "Error registering ancient one";
        return false;
    }
    return true;
}

bool Game::registerFieldOption(AH::Common::FieldData::FieldID fId, QString opId)
{
    m_fieldOptionMap[fId] << opId;
    return true;
}

void Game::increaseTerrorLevel(int amount)
{
    setTerrorLevel(m_terrorLevel + amount);
}

void Game::setTerrorLevel(int val)
{
    int amount = val - m_terrorLevel;
    m_terrorLevel = val;
    int realTL = m_context.getGameProperty(PropertyValue::Game_TerrorLevel).finalVal();

    // Adjust so that real TL is never > 10
    if (realTL > 10) {
        int diff = realTL - 10;
        m_terrorLevel -= diff;
        realTL = m_context.getGameProperty(PropertyValue::Game_TerrorLevel).finalVal();
        Q_ASSERT(realTL == 10);
        m_ancientOne->increaseDoomTrack(diff);
    }

    if (realTL >= m_context.getGameProperty(PropertyValue::Game_CloseGeneralStoreTerrorLevel).finalVal()) {
        m_board->field(AH::Common::FieldData::RT_GeneralStore)->lock(GameField::LOCK_TERROR);
    } else {
        m_board->field(AH::Common::FieldData::RT_GeneralStore)->unlock(GameField::LOCK_TERROR);
    }

    if (realTL >= m_context.getGameProperty(PropertyValue::Game_CloseCuriosityShoppeTerrorLevel).finalVal()) {
        m_board->field(AH::Common::FieldData::NS_CuriositieShoppe)->lock(GameField::LOCK_TERROR);
    } else {
        m_board->field(AH::Common::FieldData::NS_CuriositieShoppe)->unlock(GameField::LOCK_TERROR);
    }

    if (realTL >= m_context.getGameProperty(PropertyValue::Game_CloseYeOldeMagickShoppeTerrorLevel).finalVal()) {
        m_board->field(AH::Common::FieldData::UT_YeOldeMagickShoppe)->lock(GameField::LOCK_TERROR);
    } else {
        m_board->field(AH::Common::FieldData::UT_YeOldeMagickShoppe)->unlock(GameField::LOCK_TERROR);
    }

    if (amount > 0) {
        // Remove a random ally
        for (int i = 0; i < amount; ++i) {
            drawObject(AH::Obj_Ally);
        }

        // If TL > 10, increase

    }

    if (realTL >= m_context.getGameProperty(PropertyValue::Game_OverrunArkhamTerrorLevel).finalVal()) {
        overrunArkham();
    }

    setSettingDirty();
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

void Game::killPlayer(Player *p)
{
    m_notifier->notifyDied(p);
    removePlayer(p);
}

void Game::returnMonster(Monster *m)
{
    if (!m) return;
    if (m->field()) m->field()->removeMonster(m);
    m_monsterPool.returnToDeck(m);
}

Monster *Game::drawMonster()
{
#ifdef TEST_SCRIPT_BUILD
	QString more;
	Monster *m1 = scriptTestDrawHelper<Monster,ScriptTestDrawMonsterWidget>("Monster", m_monsterPool, &ScriptTestConfig::askDrawMonster, false, &more);
	if (m1) {
		int i = more.toInt();
		if (i > 0) m1->setDimension(static_cast<AH::Dimension>(i));
		return m1;
	}
#endif

    m_monsterPool.shuffle();
    int ct = m_monsterPool.size();
    do {
        Monster *m = m_monsterPool.draw();
        if (!m) return NULL;
        if (m->attributes().testFlag(Monster::Mask)) {
            if (context().getGameProperty(PropertyValue::Game_AllowMaskMonster).finalVal() > 0) {
                return m;
            } else {
                m_monsterPool.returnToDeck(m);
            }
        } else {
            return m;
        }
    } while (--ct > 0);
    return NULL;
}

MythosCard *Game::drawMythos()
{
#ifdef TEST_SCRIPT_BUILD
    MythosCard *m = scriptTestDrawHelper("Mythos", m_mythosDeck, &ScriptTestConfig::askDrawMythos, true);
#else
    MythosCard *m = m_mythosDeck.draw();
#endif
    m->resolveDynamicAttributes();
    return m;
}

void Game::returnMythos(MythosCard *m)
{
    m_mythosDeck.returnToDeck(m);
}

GameObject *Game::drawObject(AH::GameObjectType t)
{
    if (!m_objectDecks.contains(t)) {
        return NULL;
    }

#ifdef TEST_SCRIPT_BUILD
    GameObject *ret = scriptTestDrawHelper(ScriptTestConfig::nameForObjectType(t), m_objectDecks[t], [t](){return ScriptTestConfig::askDraw(t);}, true);
#else
    GameObject *ret = m_objectDecks[t].draw();
#endif
    if (ret->isInfinite()) {
        GameObject *copy = ret->clone();
        m_objectDecks[t].returnToDeck(ret);
        ret = copy;
    }
    return ret;
}

GameObject *Game::drawSpecificObject(QString id)
{
    foreach (AH::GameObjectType t, m_objectDecks.keys()) {
        GameObject *o = drawSpecificObject(id, t);
        if (o) return o;
    }
    return NULL;
}

GameObject *Game::drawSpecificObject(QString id, AH::GameObjectType t)
{
    GameObject *o = m_objectDecks[t].drawSpecificByTypeId(id);
    if (o) {
        if (o->isInfinite()) {
            GameObject *copy = o->clone();
            m_objectDecks[t].returnToDeck(o);
            return copy;
        }
        return o;
    }

    return NULL;
}

void Game::returnObject(GameObject *o)
{
    if (!o) return;
    if (o->owner() != NULL) {
        o->owner()->removeFromInventory(o);
        o->setOwner(NULL);
    }
    if (o->isInfinite()) {
        // Infinite objects are never taken out of deck. No longer used
        if (QObject *qo = dynamic_cast<QObject*> (o)) qo->deleteLater();
        else delete o;
    } else {
        m_objectDecks[o->type()].returnToDeck(o);
    }
}

int Game::drawableObjectCount(AH::GameObjectType t)
{
    return m_objectDecks[t].size();
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
        // Place doom token
        m_ancientOne->increaseDoomTrack();

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

void Game::overrunArkham()
{
    // TODO?
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
#ifdef TEST_SCRIPT_BUILD
    ArkhamEncounter *enc = scriptTestDrawHelper("Arkham Encounter", m_arkEncDecks[field], &ScriptTestConfig::askDrawArkhamEncounter, true);
#else
    ArkhamEncounter *enc = m_arkEncDecks[field].draw();
#endif
    if (enc) {
        m_arkEncDecks[field].returnToDeck(enc);
    }
    return enc;
}

OtherWorldEncounter *Game::drawOtherWorldEncounter(AH::Common::FieldData::FieldID field)
{
    AH::OtherWorldColors colors = m_board->colorsForOtherWorld(field);
    Q_ASSERT_X(colors != AH::OWC_NoColor, "Game::drawOtherWorldEncounter", "Field has no colors. Is it an other world field?");

#ifdef TEST_SCRIPT_BUILD
    OtherWorldEncounter *e1 = scriptTestDrawHelper("Other World Encoutner", m_owEncDeck, &ScriptTestConfig::askDrawOtherWorldEncounter, false);
    if (e1) return e1;
#endif

    // Sanity check...
    int ct = m_owEncDeck.size();

    // Draw until matching color is found
    OtherWorldEncounter *e = NULL;
    do {
        ct--;
        e = m_owEncDeck.draw();
        m_owEncDeck.returnToDeck(e);
        // TEST: This would allow multi colored cards
        //if ((colors & e->color()) != 0) {
        if (colors.testFlag(e->color())) {
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

PropertyModificationList Game::getGameModifiers()
{
    PropertyModificationList ret;
    if (m_environment) {
        ret = m_environment->getModifications();
    }
    ret += ancientOne()->getSlumberModifications();
    return ret;
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

    if (isSettingDirty()) {
        setSettingDirty(false);
        sendSetting();
    }

    if (m_ancientOne->isDirty()) {
        m_ancientOne->setDirty(false);
        m_invalidatedObjects << m_ancientOne->id();
    }

    foreach (Character *c, m_registry->allCharacters()) {
        c->commitDamage();
        if (c->isDirty()) {
            c->setDirty(false);
            m_invalidatedObjects << c->id();

            // Force Update character for player
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

        MythosCard *mc = m_registry->findMythosById(r.second);
        if (mc) return describeObject(qMakePair(AH::Common::RequestObjectsData::Mythos, r.second));

        AncientOne *ao = m_registry->findAncientOneById(r.second);
        if (ao) return describeObject(qMakePair(AH::Common::RequestObjectsData::AncientOne, r.second));

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
    case AH::Common::RequestObjectsData::Mythos:
    {
        MythosCard *mc = m_registry->findMythosById(r.second);
        if (mc) {
            d.data << *(mc->data());
            d.id = mc->id();
            d.type = AH::Common::RequestObjectsData::Mythos;
        }
        break;
    }
    case AH::Common::RequestObjectsData::AncientOne:
    {
        AncientOne *ao = m_registry->findAncientOneById(r.second);
        if (ao) {
            d.data << *(ao->data());
            d.id = ao->id();
            d.type = AH::Common::RequestObjectsData::AncientOne;
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
    int dim = RandomSource::instance().nextUint(0, 8);
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

    // TODO: Check if stable...

    AH::Common::FieldData::FieldID ret = static_cast<AH::Common::FieldData::FieldID> (id);
    Q_ASSERT(m_board->field(ret) != NULL);
    Q_ASSERT(m_board->field(ret)->type() == AH::Common::FieldData::Location);
    return ret;
}

bool Game::setRumor(MythosCard *r)
{
    if (r == NULL) {
        //returnMythos(m_rumor);
        m_rumor = NULL;
        setSettingDirty();
        return true;
    }
    if (m_rumor) {
        return false;
    }
    if (r->type() != AH::Common::MythosData::Rumor) {
        return false;
    }
    m_rumor = r;
    setSettingDirty();
    return true;
}

bool Game::setEnvironment(MythosCard *e)
{
    // If old or new environment has monster modifiers,
    // client cached monsters must be invalidated...
    bool invalidateMonsters = false;

    if (m_environment) {
        MythosCard *env = m_environment;
        m_environment = NULL;
        env->cleanup();
        if (env->hasMonsterModifications()) {
            invalidateMonsters = true;
        }
        returnMythos(env);
        setSettingDirty();
    }
    if (e) {
        if (e->type() != AH::Common::MythosData::Environment) {
            return false;
        }
    }
    m_environment = e;
    if (m_environment && m_environment->hasMonsterModifications()) {
        invalidateMonsters = true;
    }
    setSettingDirty();

    if (invalidateMonsters) {
        m_notifier->objectTypeInvalidated(AH::Common::RequestObjectsData::Monster);
    }

    return true;
}

int Game::getSpecialActionNumber()
{
    if (m_specialActionNrs.isEmpty()) {
        m_specialActionNrs << m_nextSpecialActionNr++;
    }
    return m_specialActionNrs.takeFirst();
}

void Game::returnSpecialActionNumber(int nr)
{
    if (nr > 0) {
        m_specialActionNrs << nr;
        qSort(m_specialActionNrs);
    }
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
    /*
    setEnvironment(m_mythosDeck.drawSpecificById("MY_TheFestival"));
    m_monsterPool.shuffle();
    m_board->field(AH::Common::FieldData::DT_Downtown)->placeMonster(m_monsterPool.drawSpecificByTypeId("MO_Cultist"));
    m_board->field(AH::Common::FieldData::SS_Southside)->placeMonster(m_monsterPool.drawSpecificByTypeId("MO_Byakhee"));
    */
}

void Game::chooseInvestigators()
{
    ChooseInvestigator ci(this);
    ci.execute();
}

void Game::chooseAncientOne()
{
    foreach (AncientOne *a, m_registry->allAncientOnes()) {
        m_ancientOnePool.addCard(a);
    }

    // TODO: Let user decide?
    m_ancientOnePool.shuffle();
#ifdef TEST_SCRIPT_BUILD
    m_ancientOne = scriptTestDrawHelper("Ancient One", m_ancientOnePool, &ScriptTestConfig::askDrawAncientOne, true);
#else
    m_ancientOne = m_ancientOnePool.draw();
#endif
    setSettingDirty(true);
}

void Game::initInvestigators()
{
    // FIXED POSSESSION
    foreach (Character *c, m_registry->allCharacters())
    {
        m_context.m_player = playerForCharacter(c);
        initCharacterFixedPossession(c);
        m_context.m_player = NULL;
    }

    // RANDOM POSSESSION
    foreach (Character *c, m_registry->allCharacters())
    {
        m_context.m_player = playerForCharacter(c);
        initCharacterRandomPossession(c);
        m_context.m_player = NULL;
    }

    // START FIELD
    foreach (Character *c, m_registry->allCharacters())
    {
        m_context.m_player = playerForCharacter(c);
        m_board->field(c->investigator()->startFieldId())->placeCharacter(c);
        m_context.m_player = NULL;
    }

    commitUpdates();

    // INITIAL FOCUS
    foreach (Player *p, m_registry->allPlayers())
    {
        FocusAction fa;
        int amount = 100;
        m_context.m_player = p;
        fa.executeOnPlayer(p, amount);
        m_context.m_player = NULL;
    }
}

void Game::initCharacterFixedPossession(Character *c)
{
    // Will be dirty after all of this...
    c->setDirty();

    foreach (QString tid, c->investigator()->fixedPossesionObjectIds())
    {
        GameObject *obj = NULL;
        const GameObject *proto = m_registry->findObjectPrototypeByType(tid);
        if (proto) {
            obj = this->drawSpecificObject(tid, proto->type());
            if (obj) {
                c->addToInventory(obj);
            } else {
                qWarning() << "Cannot resolve fixed possession: No more objects of type" << tid;
            }
        } else {
            qCritical() << "Cannot resolve fixed possession: Object type" << tid << "not registered";
        }
    }
}

void Game::initCharacterRandomPossession(Character *c)
{
    // Will be dirty after all of this...
    c->setDirty();

    foreach (AH::ObjectTypeCount otc, c->investigator()->randomPossesions()) {
        // TODO: MUST BE INTERACTION! IF THERE IS A SPECIAL ABILITY BY INVESTIGATOR,
        // HE MIGHT CHOOSE FROM VARIOUS CARDS!
        for (int i = 0; i < otc.amount; ++i) {
            GameObject *obj = this->drawObject(otc.type);
            if (obj)
                c->addToInventory(obj);
            else
                qWarning() << "No more objects available for random possessions";
        }
    }
}

void Game::replacePlayerCharacter(Player *p, Investigator *i)
{
    Character *oldChar = p->getCharacter();

    // Return everything character possesses
    QList<GameObject *> inv = oldChar->inventory();
    inv.detach();
    foreach (GameObject *obj, inv) {
        returnObject(obj);
    }

    // Instantiate new character and set it
    Character *newChar = i->instantiate();
    p->setCharacter(newChar);
    registerCharacter(newChar);

    // Remove old character
    oldChar->field()->removeCharacter(oldChar);
    m_registry->removeCharacter(oldChar);
    delete oldChar;

    // Initialize new character
    initCharacterFixedPossession(newChar);
    initCharacterRandomPossession(newChar);
    m_board->field(newChar->investigator()->startFieldId())->placeCharacter(newChar);

    p->playerCharacterInstantiated(p);

    // Initial Focus again
    FocusAction fa;
    int amount = 100;
    fa.executeOnPlayer(p, amount);
}

void Game::preventDamageHelper(Player *p, int &damageStamina, int &damageSanity, int &preventStamina, int &preventSanity)
{
    preventStamina = preventSanity = 0;
    Character *c = p->getCharacter();

    QList<GameOption *> options;

    do {
        foreach (GameObject *obj, c->inventory()) {
            if (obj->hasPreventedDamage() || obj->isExhausted()) continue;
            foreach (GameOption *opt, obj->getOptions()) {
                if (opt->phases().testFlag(AH::CommitDamagePhase)) {
                    PreventDamageOption *pOpt = dynamic_cast<PreventDamageOption *> (opt);
                    if (!pOpt) continue;
                    pOpt->setSource(obj);
                    if (!pOpt->isAvailable()) continue;
                    pOpt->updateOverrideDescription(damageStamina, damageSanity);

                    if (damageStamina > 0 && pOpt->canPreventStamina()) {
                        options << opt;
                        continue;
                    }
                    if (damageSanity > 0 && pOpt->canPreventSanity()) {
                        options << opt;
                    }
                }
            }
        }

        if (options.isEmpty()) {
            break;
        }

        options << GamePhase::getSkipOption();

        GameOption *chosen = p->chooseOption(options);
        if (!chosen) {
            break;
        }
        if (chosen == GamePhase::getSkipOption()) {
            break;
        }

        PreventDamageOption *pChosen = dynamic_cast<PreventDamageOption *> (chosen);
        if (!pChosen) {
            break;
        }
        pChosen->execute(damageStamina, damageSanity);

        preventStamina += pChosen->preventedStamina();
        preventSanity += pChosen->preventedSanity();
        damageStamina -= pChosen->preventedStamina();
        damageSanity -= pChosen->preventedSanity();

        foreach (GameOption *o, options) {
            if (PreventDamageOption *pO = dynamic_cast<PreventDamageOption*>(o)) {
                pO->reset();
            }
        }

        options.clear();

        if (damageSanity <= 0 && damageStamina <= 0) {
            break;
        }

    } while (true);

    foreach (GameOption *o, options) {
        if (PreventDamageOption *pO = dynamic_cast<PreventDamageOption*>(o)) {
            pO->reset();
        }
    }

    foreach (GameObject *obj, c->inventory()) {
        obj->resetPreventDamage();
    }

}

//private below:

void Game::upkeep()
{
    m_notifier->gamePhaseChaned(AH::Upkeep);
    executePlayerPhase(m_phases[UpkeepIndex], AH::Upkeep);
}

void Game::movement()
{
    m_notifier->gamePhaseChaned(AH::Movement);
    executePlayerPhase(m_phases[MovementIndex], AH::Movement);
}

void Game::arkhamEncountery()
{
    m_notifier->gamePhaseChaned(AH::ArkhamEncountery);
    executePlayerPhase(m_phases[ArkhamEncounteryIndex], AH::ArkhamEncountery);
}

void Game::otherWorldEncountery()
{
    m_notifier->gamePhaseChaned(AH::OtherWorldEncountery);
    executePlayerPhase(m_phases[OtherWorldEncounteryIndex], AH::OtherWorldEncountery);
}

void Game::mythos()
{
    m_notifier->gamePhaseChaned(AH::Mythos);
    // Not a player phase
    m_context = GameContext(this, getFirstPlayer(), NULL, AH::Mythos);
    m_phases[MythosIndex]->execute();
}

void Game::attackAncientOne()
{
    executePlayerPhase(m_phases[AttackAncientOneIndex], AH::EndFightPhase);
}

void Game::ancientOneAttack()
{
    // Not a player phase
    foreach (Player *p, m_playerList) {
        if (p->isActive()) {
            m_context = GameContext(this, p, NULL, AH::EndFightPhase);
            m_phases[AncientOneAttackIndex]->execute();
        }
    }
}

void Game::executePlayerPhase(GamePhase *ph, AH::GamePhase phase)
{
    foreach (Player *p, m_registry->allPlayers()) {
        m_notifier->currentPlayerChanged(p);
        m_context = GameContext(this, p, NULL, phase);
        ph->execute();
    }
}

bool Game::playRound()
{
    upkeep();
    if (!postRoundChecks()) return false;

    movement();
    if (!postRoundChecks()) return false;

    arkhamEncountery();
    if (!postRoundChecks()) return false;

    otherWorldEncountery();
    if (!postRoundChecks()) return false;

    mythos();
    if (!postRoundChecks()) return false;

    nextRound();
    return true;
    //TEST
    //return false;
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

bool Game::postRoundChecks()
{
    cleanupDeactivatedPlayers();
    commitUpdates();
    return checkGameState() == GS_Continue;
}

Game::GameState Game::checkGameState()
{
    // TEST
    //return GS_AwakeAncientOne;


    if (m_context.phase() == AH::EndFightPhase) {
        // TODO: nothing? (all handled in endFight())
    } else {
        // Count sealed gates and open gates
        int ctSealed = 0;
        int ctOpen = 0;
        foreach (GameField *gf, m_board->fields(AH::Common::FieldData::Location)) {
            if (gf->isSealed()) ctSealed++;
            if (gf->gate() != NULL) ctOpen++;
        }

        if (ctSealed >= m_context.getGameProperty(PropertyValue::Game_SealedGatesToWin).finalVal()) {
            return GS_Win_SealedGates;
        }

        if (ctOpen == 0) {
            // Count collected trophies
            int ctTrophies = 0;
            foreach (Player *p, m_playerList) {
                ctTrophies += p->getCharacter()->gateMarkerIds().count();
            }

            if (ctTrophies >= m_context.getGameProperty(PropertyValue::Game_GateTrophiesToWin).finalVal()) {
                return GS_Win_ClosedGates;
            }
        }

        // Awake ancient one?
        if (m_ancientOne->doomValue() >= m_ancientOne->doomTrack()) {
            return GS_AwakeAncientOne;
        }
    }

    return GS_Continue;
}

void Game::won(Game::GameState gs)
{
    Q_UNUSED(gs)
    m_notifier->notifyWon("You have won!");
}

void Game::lost(Game::GameState gs)
{
    Q_UNUSED(gs)
    m_notifier->notifyLost("You have lost!");
}

void Game::awakeAncientOne()
{
    m_ancientOne->awake();
    endFight();
}

void Game::endFight()
{
    // Remove all items that are to remove when AO awakes
    foreach (Player *p, getPlayers()) {
        if (p->isActive()) {
            QList<GameObject *> lst = p->getCharacter()->inventory();
            foreach (GameObject *obj, lst) {
                if (obj->getAttributes().testFlag(GameObject::DiscardOnEndFight)) {
                    p->getCharacter()->removeFromInventory(obj);
                }
            }
        }
    }
    commitUpdates();

    // TODO: Place all characters at End Fight Field (required for Trade)

    upkeep();
    nextRound();

    // TODO: Trade



    do {
        // Investigators attack
        attackAncientOne();
        if (m_ancientOne->isDefeated()) {
            this->won(GS_Win_DefeatedAncientOne);
            return;
        }

        // AncientOne attacks
        ancientOneAttack();

        // Check if any investigators are left
        if (countActivePlayers() == 0) {
            this->lost(GS_Lost);
            return;
        }

        if (!m_ancientOne->postAttack()) {
            this->lost(GS_Lost);
            return;
        }
        m_ancientOne->newAttackRound();
    } while (true);
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

int Game::countActivePlayers() const
{
    int ct = 0;
    QList<Player *> lst = m_registry->allPlayers();
    foreach (Player *p, lst) {
        if (p->isActive()) {
            ct++;
        }
    }
    return ct;
}

void Game::sendBoard()
{
    m_notifier->sendBoard(m_board);
}

void Game::sendSetting()
{
    AH::Common::GameSettingData d;
    d.setTerrorLevel(m_terrorLevel);
    if (m_ancientOne)
        d.setAncientOneId(m_ancientOne->id());
    if (m_rumor)
        d.setRumorId(m_rumor->id());
    if (m_environment)
        d.setEnvironmentId(m_environment->id());
    m_notifier->sendSetting(d);
}
