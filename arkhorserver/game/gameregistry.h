#ifndef GAMEREGISTRY_H
#define GAMEREGISTRY_H

#include <QMap>
#include <QList>

class GameAction;
class GameOption;
class GameObject;
class Player;
class Character;
class Investigator;
class Monster;
class ArkhamEncounter;
class OtherWorldEncounter;
class MythosCard;
class Gate;
class AncientOne;

class GameRegistry
{
public:
    GameRegistry();
    ~GameRegistry();

    bool registerAction(GameAction *a);
    bool registerOption(GameOption *o);
    bool registerMultiObject(GameObject *o, qint32 ct = 1);
    bool registerSingleObject(GameObject *o);
    bool registerInvestigator(Investigator *i);
    bool registerPlayer(Player *p);
    bool registerCharacter(Character *c);
    bool registerMonster(Monster *m, int ct = 1);
    bool registerArkhamEncounter(ArkhamEncounter *ae);
    bool registerOtherWorldEncounter(OtherWorldEncounter *owe);
    bool registerMythosCard(MythosCard *m);
    bool registerGate(Gate *g);
    bool registerAncientOne(AncientOne *ao);

    bool removePlayer(Player *p);
    bool removeCharacter(Character *c);

    GameAction *findActionById(QString id);
    GameOption *findOptionById(QString id);
    Investigator *findInvestigatorById(QString id);
    Player *findPlayerById(QString id);
    Character *findCharacterById(QString id);
    Monster *findMonsterById(QString id);
    Gate *findGateById(QString id);
    MythosCard *findMythosById(QString id);
    AncientOne *findAncientOneById(QString id);

    const GameObject *findObjectPrototypeByType(QString tid);
    GameObject *findObjectInstanceByType(QString tid);
    GameObject *findObjectById(QString id);

    QList<GameObject *> allObjects();
    QList<Investigator *> allInvestigators();
    QList<Player *> allPlayers();
    QList<Character *> allCharacters();
    QList<Monster *> allMonsters();
    QList<ArkhamEncounter *> allArkhamEncounters();
    QList<OtherWorldEncounter *> allOtherWorldEncounters();
    QList<MythosCard *> allMythosCards();
    QList<AncientOne *> allAncientOnes();

    bool resolveDependencies();

    static const QString TempObjectId;

private:
    template <typename T>
    bool doRegisterItem(T *t, QMap<QString, T*> &m);
    int m_nextActionId;
    int m_nextOptionId;
    int m_nextMythosId;
    int m_nextGateId;

private:
    QMap<QString, GameAction*> m_actions;
    QMap<QString, GameOption*> m_options;
    QMap<QString, Investigator*> m_investigators;
    QMap<QString, Player*> m_players;
    QMap<QString, Character*> m_characters;
    QMap<QString, GameObject*> m_objects;
    QMap<QString, GameObject*> m_objectTypes;
    QMap<QString, Monster*> m_monsters;
    QMap<QString, Monster*> m_monsterTypes;
    QList<ArkhamEncounter *> m_arkEncounters;
    QList<OtherWorldEncounter *> m_otherEncounters;
    QMap<QString, MythosCard *> m_mythosCards;
    QMap<QString, Gate *> m_gates;
    QMap<QString, AncientOne *> m_ancientOnes;
};

template <typename T>
bool GameRegistry::doRegisterItem(T *t, QMap<QString, T*> &m)
{
    if (!t) return false;
    if (t->id().isEmpty()) return false;
    if (m.contains(t->id())) return false;
    m.insert(t->id(), t);
    return true;
}

#endif // GAMEREGISTRY_H
