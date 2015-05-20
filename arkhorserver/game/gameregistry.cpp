#include "gameregistry.h"
#include "gameaction.h"
#include "gameoption.h"
#include "gameobject.h"
#include "investigator.h"
#include "character.h"
#include "player.h"
#include "monster.h"
#include "arkhamencounter.h"
#include "otherworldencounter.h"
#include "mythoscard.h"
#include "gate.h"
#include "ancientone.h"

GameRegistry::GameRegistry()
:   m_nextActionId(0),
    m_nextOptionId(0),
    m_nextMythosId(0),
    m_nextGateId(0)
{
}

GameRegistry::~GameRegistry()
{
    qDeleteAll(m_actions.values());
    qDeleteAll(m_options.values());
    qDeleteAll(m_investigators.values());
    qDeleteAll(m_players.values());
    qDeleteAll(m_characters.values());
    qDeleteAll(m_objects.values());
    qDeleteAll(m_objectTypes.values());
    qDeleteAll(m_monsters.values());
    qDeleteAll(m_monsterTypes.values());
    qDeleteAll(m_arkEncounters);
    qDeleteAll(m_otherEncounters);
    qDeleteAll(m_mythosCards.values());
    qDeleteAll(m_gates);
}

bool GameRegistry::registerAction(GameAction *a)
{
    if (a->id().isEmpty()) {
        QString genId = QString("AC_GEN_%1").arg(++m_nextActionId);
        a->setId(genId);
    }
    return doRegisterItem(a, m_actions);
}

bool GameRegistry::registerOption(GameOption *o)
{
    if (o->id().isEmpty()) {
        QString genId = QString("OP_GEN_%1").arg(++m_nextOptionId);
        o->setId(genId);
    }
    return doRegisterItem(o, m_options);
}

bool GameRegistry::registerMultiObject(GameObject *o, int ct)
{
    if (!o) return false;
    if (o->typeId().isEmpty()) return false;
    if (m_objectTypes.contains(o->typeId())) return false;
    m_objectTypes.insert(o->typeId(), o);

    for (int i = 0; i < ct; ++i) {
        GameObject *io = o->clone();
        io->setId(QString("%2:%1").arg(i).arg(io->typeId()));
        m_objects.insert(io->id(), io);
    }
    return true;
}

bool GameRegistry::registerSingleObject(GameObject *o)
{
    if (!o) return false;
    if (o->typeId().isEmpty()) return false;
    if (m_objectTypes.contains(o->typeId())) return false;

    m_objectTypes.insert(o->typeId(), o);
    o->setId(o->typeId());
    m_objects.insert(o->id(), o);
    return true;
}

bool GameRegistry::registerInvestigator(Investigator *i)
{
    return doRegisterItem(i, m_investigators);
}

bool GameRegistry::registerPlayer(Player *p)
{
    return doRegisterItem(p, m_players);
}

bool GameRegistry::registerCharacter(Character *c)
{
    return doRegisterItem(c, m_characters);
}

bool GameRegistry::registerMonster(Monster *m, int ct)
{
    if (!m) return false;
    if (m->typeId().isEmpty()) return false;
    if (!m_monsterTypes.contains(m->typeId()))
        m_monsterTypes.insert(m->typeId(), m);

    for (int i = 0; i < ct; ++i) {
        Monster *im = m->clone();
        im->setId(QString("%2:%1").arg(i).arg(im->typeId()));
        m_monsters.insert(im->id(), im);
    }
    return true;
}

bool GameRegistry::registerArkhamEncounter(ArkhamEncounter *ae)
{
    m_arkEncounters << ae;
    return true;
}

bool GameRegistry::registerOtherWorldEncounter(OtherWorldEncounter *owe)
{
    m_otherEncounters << owe;
    return true;
}

bool GameRegistry::registerMythosCard(MythosCard *m)
{
    if (m->id().isEmpty()) {
        m->setId(QString("MY_GEN_%1").arg(++m_nextMythosId));
    }
    if (m_mythosCards.contains(m->id())) {
        return false;
    }
    m_mythosCards.insert(m->id(), m);
    return true;
}

bool GameRegistry::registerGate(Gate *g)
{
    if (g) {
        if (g->id().isEmpty()) {
            g->setId(QString("GT_GEN_%1").arg(++m_nextGateId));
        }
        m_gates[g->id()] = g;
        return true;
    }
    return false;
}

bool GameRegistry::removePlayer(Player *p)
{
    if (!p) return false;
    if (p->id().isEmpty()) return false;
    if (!m_players.contains(p->id())) return false;
    m_players.remove(p->id());
    return true;
}

bool GameRegistry::registerAncientOne(AncientOne *ao)
{
    if (m_ancientOnes.contains(ao->id())) return false;
    m_ancientOnes[ao->id()] = ao;
    return true;
}

GameAction *GameRegistry::findActionById(QString id)
{
    return m_actions.value(id);
}

GameOption *GameRegistry::findOptionById(QString id)
{
    return m_options.value(id);
}

const GameObject *GameRegistry::findObjectPrototypeByType(QString tid)
{
    return m_objectTypes.value(tid);
}

GameObject *GameRegistry::findObjectById(QString id)
{
    return m_objects.value(id);
}

Investigator *GameRegistry::findInvestigatorById(QString id)
{
    return m_investigators.value(id);
}

Player *GameRegistry::findPlayerById(QString id)
{
    return m_players.value(id);
}

Character *GameRegistry::findCharacterById(QString id)
{
    return m_characters.value(id);
}

Monster *GameRegistry::findMonsterById(QString id)
{
    return m_monsters.value(id);
}

Gate *GameRegistry::findGateById(QString id)
{
    return m_gates.value(id);
}

QList<GameObject *> GameRegistry::allObjects()
{
    return m_objects.values();
}

QList<Investigator *> GameRegistry::allInvestigators()
{
    return m_investigators.values();
}

QList<Player *> GameRegistry::allPlayers()
{
    return m_players.values();
}

QList<Character *> GameRegistry::allCharacters()
{
    return m_characters.values();
}

QList<Monster *> GameRegistry::allMonsters()
{
    return m_monsters.values();
}

QList<ArkhamEncounter *> GameRegistry::allArkhamEncounters()
{
    return m_arkEncounters;
}

QList<OtherWorldEncounter *> GameRegistry::allOtherWorldEncounters()
{
    return m_otherEncounters;
}

QList<MythosCard *> GameRegistry::allMythosCards()
{
    return m_mythosCards.values();
}

QList<AncientOne *> GameRegistry::allAncientOnes()
{
    return m_ancientOnes.values();
}

bool GameRegistry::resolveDependencies()
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

    // Resolve Encounters:
    foreach (ArkhamEncounter *ae, m_arkEncounters) {
        ok &= ae->resolveDependencies(this);
    }
    foreach (OtherWorldEncounter *owe, m_otherEncounters) {
        ok &= owe->resolveDependencies(this);
    }

    // Resolve Mythos
    foreach (MythosCard *mc, m_mythosCards) {
        ok &= mc->resolveDependencies();
    }

    return ok;
}

