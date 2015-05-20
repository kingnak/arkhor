#ifndef GAME_H
#define GAME_H

#include <ahglobal.h>
#include "gamenotifier.h"
#include "gameinteractor.h"
#include "gamecontext.h"
#include "propertymodifier.h"
#include "cost.h"
#include <QList>
#include <QVector>
#include <QMap>
#include <QReadWriteLock>
#include "gameboard.h"
#include "gameregistry.h"
#include "deck.hpp"
#include <objectdata.h>

class GamePhase;
class GameBoard;
class GameObject;
class Investigator;
class Player;
class ArkhamEncounter;
class OtherWorldEncounter;

class Game : public QObject
{
    Q_OBJECT
public:
    Game();
    ~Game();

    void invoke(const char *method);

    void initPhases();

    static Game *instance();
    QList<Player *> getPlayers();

    void registerInvestigator(Investigator *i);
    QList<Investigator *> allInvestigators() const;

    void registerCharacter(Character *c);
    void registerAction(GameAction *a);
    void registerOption(GameOption *o);
    void registerObject(GameObject *o, quint32 count = 1);
    void registerArkhamEnconutry(ArkhamEncounter *a);
    void registerOtherWorldEncountery(OtherWorldEncounter *e);
    void registerMonster(Monster *m, quint32 count = 1);
    bool resolveDependencies();

    void registerFieldOption(AH::Common::FieldData::FieldID fId, QString opId);

    /*
    enum RegisteredType {
        RT_None,
        RT_Action,
        RT_Option,
        RT_Object,
        RT_Investigator,
        RT_Character,
        RT_Player
    };

    RegisteredType getGameItemType(const QString &id) const;
    */

    /*
    GameAction *findActionById(const QString &id) const;
    GameOption *findOptionById(const QString &id) const;
    GameObject *findObjectById(const QString &id) const;
    */

    GameRegistry *registry() { return m_registry; }

    Player *getFirstPlayer();
    Player *getCurrentPlayer();

    bool addPlayer(Player *p);

    void removePlayer(Player *p);

    void returnMonster(Monster *m);

    GameContext &context();
    GameBoard *board();

    GameNotifier *notifier();

    ArkhamEncounter *drawArkhamEncounter(AH::Common::FieldData::FieldID field);
    OtherWorldEncounter *drawOtherWorldEncounter(AH::Common::FieldData::FieldID field);

    PropertyModificationList getGameModifiers() { return PropertyModificationList(); }

    int getGateSealClueCost() const { return 5; }

    void boardDirty();
    void characterDirty(Character *c);

    void commitUpdates();

    Player *playerForCharacter(Character *c);

    AH::Common::DescribeObjectsData describeObjects(const AH::Common::RequestObjectsData &reqs) const;

protected:
    void initBoard();
    void initDecks();
    void initMonsters();
    void chooseInvestigators();
    void initInvestigators();

private:
    Q_INVOKABLE void start();
    void play();

    void upkeep();
    void movement();
    void arkhamEncountery();
    void otherWorldEncountery();
    void mythos();

    void executePlayerPhase(int idx, AH::GamePhase phase);

    void playRound();
    void nextRound();

    void cleanupDeactivatedPlayers();

    void sendBoard();

private:
    enum {
        UpkeepIndex = 0,
        MovementIndex,
        ArkhamEncounteryIndex,
        OtherWorldEncounteryIndex,
        MythosIndex
    };

private:
    // Mappings for registry resolving
    QMap<AH::Common::FieldData::FieldID, QStringList> m_fieldOptionMap;
    QMap<AH::Common::FieldData::FieldID, QList<ArkhamEncounter *> > m_arkEnc;
    QList<OtherWorldEncounter *> m_owEnc;

    QMap<AH::GameObjectType, Deck<GameObject> > m_objectDecks;
    QMap<AH::Common::FieldData::FieldID, Deck<ArkhamEncounter> > m_arkEncDecks;
    Deck<OtherWorldEncounter> m_owEncDeck;

    Deck<Monster> m_monsterPool;

    QList<Player *> m_playerList;
    QVector<GamePhase *> m_phases;
    static Game *s_instance;

    GameContext m_context;
    GameBoard *m_board;
    GameNotifier *m_notifier;
    GameRegistry *m_registry;

    int m_nextPlayerId;
    bool m_started;

    mutable QReadWriteLock m_lock;
};

#define gGame (Game::instance())

#endif // GAME_H
