#include "gameregistry.h"
#include "gameaction.h"
#include "gameoption.h"
#include "gameobject.h"
#include "investigator.h"
#include "character.h"
#include "player.h"
#include "monster.h"

GameRegistry::GameRegistry()
{
}

bool GameRegistry::registerAction(GameAction *a)
{
    return doRegisterItem(a, m_actions);
}

bool GameRegistry::registerOption(GameOption *o)
{
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

bool GameRegistry::removePlayer(Player *p)
{
    if (!p) return false;
    if (p->id().isEmpty()) return false;
    if (!m_players.contains(p->id())) return false;
    m_players.remove(p->id());
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

    return ok;
}

