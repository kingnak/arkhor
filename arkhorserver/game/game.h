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
class MythosCard;
class AncientOne;

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

    bool registerInvestigator(Investigator *i);
    QList<Investigator *> allInvestigators() const;
    Investigator *drawInvestigator();
    Investigator *drawSpecificInvestigator(QString id);
    void returnInvestigator(Investigator *i);
    void returnEncounter(ArkhamEncounter *e);
    void returnEncounter(OtherWorldEncounter *e);

    bool registerCharacter(Character *c);
    bool registerAction(GameAction *a);
    bool registerOption(GameOption *o);
    bool registerObject(GameObject *o, qint32 count = 1);
    bool registerArkhamEnconutry(ArkhamEncounter *a);
    bool registerOtherWorldEncountery(OtherWorldEncounter *e);
    bool registerMonster(Monster *m, quint32 count = 1);
    bool registerMythos(MythosCard *m);
    bool registerAncientOne(AncientOne *ao);
    bool resolveDependencies();

    bool registerFieldOption(AH::Common::FieldData::FieldID fId, QString opId);

    int terrorLevel() const { return m_terrorLevel; }
    void increaseTerrorLevel(int amount = 1);
    void setTerrorLevel(int val);
    bool isOverrun();

    void setSettingDirty(bool dirty = true) { m_settingDirty = dirty; }
    bool isSettingDirty() const { return m_settingDirty; }
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

    AncientOne *ancientOne() { return m_ancientOne; }

    bool addPlayer(Player *p);

    void removePlayer(Player *p);

    void killPlayer(Player *p);

    void returnMonster(Monster *m);
    Monster *drawMonster();

    MythosCard *drawMythos();
    void returnMythos(MythosCard *m);

    GameObject *drawObject(AH::GameObjectType t);
    GameObject *drawSpecificObject(QString id);
    GameObject *drawSpecificObject(QString id, AH::GameObjectType t);
    void returnObject(GameObject *o);
    int drawableObjectCount(AH::GameObjectType t);

    bool createGate(GameField *field);
    bool createMonster(GameField *field);
    bool putOutskirtsMonster(Monster *m);
    void returnOutskirtsMonsters();
    void closeGate(Gate *g, Character *c);
    void closeGateCleanup(Gate *g);
    bool canPlaceMonster();

    void overrunArkham();

	void requestAwakeAncientOne();

    GameContext &context();
    GameBoard *board();

    GameNotifier *notifier();

    ArkhamEncounter *drawArkhamEncounter(AH::Common::FieldData::FieldID field);
    OtherWorldEncounter *drawOtherWorldEncounter(AH::Common::FieldData::FieldID field);

    PropertyModificationList getGameModifiers();

    int getGateSealClueCost() const { return 5; }

    void boardDirty();
    void characterDirty(Character *c);
    void invalidateObject(QString id);
    void notifyTempObject(const QString &desc);

    void commitUpdates();

    Player *playerForCharacter(Character *c);

    AH::Common::DescribeObjectsData describeObjects(const AH::Common::RequestObjectsData &reqs) const;
    AH::Common::DescribeObjectsData::ObjectDescription describeObject(const AH::Common::RequestObjectsData::ObjectRequest &r) const;

    AH::Dimension randomDimension() const;
    AH::Common::FieldData::FieldID randomLocation(bool onlyStable) const;

    bool setRumor(MythosCard *r);
    MythosCard *rumor() { return m_rumor; }

    bool setEnvironment(MythosCard *e);
    MythosCard *environment() { return m_environment; }

    int getSpecialActionNumber();
    void returnSpecialActionNumber(int nr);

    void replacePlayerCharacter(Player *p, Investigator *i);

    void preventDamageHelper(Player *p, int &damageStamina, int &damageSanity, int &preventStamina, int &preventSanity);

protected:
    void initBoard();
    void initDecks();
    void initMonsters();
    void chooseInvestigators();
    void chooseAncientOne();
    void initInvestigators();

    void initCharacterFixedPossession(Character *c);
    void initCharacterRandomPossession(Character *c);


private:
    enum GameState {
        GS_Continue,
        GS_Win_ClosedGates,
        GS_Win_SealedGates,
        GS_Win_DefeatedAncientOne,
        GS_AwakeAncientOne,
        GS_Lost
    };

private:
    Q_INVOKABLE void start();
    void play();

    void upkeep();
    void movement();
    void arkhamEncountery();
    void otherWorldEncountery();
    void mythos();
    void attackAncientOne();
    void ancientOneAttack();

    void executePlayerPhase(GamePhase *ph, AH::GamePhase phase);

    bool playRound();
    void nextRound();
    bool postRoundChecks();
    GameState checkGameState();

    void won(GameState gs);
    void lost(GameState gs);
    void awakeAncientOne();
    void endFight();

    void cleanupDeactivatedPlayers();
    int countActivePlayers() const;

    void sendBoard();
    void sendSetting();

private:
    enum {
        UpkeepIndex = 0,
        MovementIndex,
        ArkhamEncounteryIndex,
        OtherWorldEncounteryIndex,
        MythosIndex,
        AttackAncientOneIndex,
        AncientOneAttackIndex
    };

private:
    // Mappings for registry resolving
    QMap<AH::Common::FieldData::FieldID, QStringList> m_fieldOptionMap;
    //QMap<AH::Common::FieldData::FieldID, QList<ArkhamEncounter *> > m_arkEnc;
    //QList<OtherWorldEncounter *> m_owEnc;

    QMap<AH::GameObjectType, Deck<GameObject> > m_objectDecks;
    QMap<AH::Common::FieldData::FieldID, Deck<ArkhamEncounter> > m_arkEncDecks;
    Deck<OtherWorldEncounter> m_owEncDeck;
    Deck<MythosCard> m_mythosDeck;
    Deck<Monster> m_monsterPool;
    Deck<AncientOne> m_ancientOnePool;
    Deck<Investigator> m_investigators;

    QList<Player *> m_playerList;
    QVector<GamePhase *> m_phases;
    static Game *s_instance;

    GameContext m_context;
    GameBoard *m_board;
    GameNotifier *m_notifier;
    GameRegistry *m_registry;

    MythosCard *m_environment;
    MythosCard *m_rumor;

    AncientOne *m_ancientOne;

    QSet<QString> m_invalidatedObjects;

    int m_nextPlayerId;
    bool m_started;

    bool m_settingDirty;
    int m_terrorLevel;

    QList<int> m_specialActionNrs;
    int m_nextSpecialActionNr;

	bool m_reqAwake;

    mutable QReadWriteLock m_lock;
};

#define gGame (Game::instance())

#endif // GAME_H
