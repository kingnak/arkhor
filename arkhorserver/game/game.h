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

class GamePhase;
class GameBoard;
class GameObject;
class Investigator;
class Player;
class ArkhamEncounter;

class Game : public QObject
{
    Q_OBJECT
public:
    Game();
    ~Game();

    void invoke(const char *method);

    void init();

    static Game *instance();
    QList<Player *> getPlayers();

    void registerInvestigator(Investigator *i);
    QList<Investigator *> allInvestigators() const;

    void registerCharacter(Character *c);
    void registerAction(GameAction *a);
    void registerOption(GameOption *o);
    void registerObject(GameObject *o, quint32 count = 1);
    void registerArkhamEnconutry(ArkhamEncounter *a);
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

    GameAction *findActionById(const QString &id) const;
    GameOption *findOptionById(const QString &id) const;
    GameObject *findObjectById(const QString &id) const;

    Player *getFirstPlayer();
    Player *getCurrentPlayer();

    bool addPlayer(Player *p);

    void removePlayer(Player *p);

    GameContext context();
    GameBoard *board();

    GameNotifier *notifier();

    PropertyModificationList getGameModifiers() { return PropertyModificationList(); }

    int getGateSealClueCost() const { return 5; }

    void boardDirty();

protected:
    void initBoard();
    void chooseInvestigators();

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
    // Registries:
    QMap<QString, Investigator *> m_investigators;
    QMap<QString, Character *> m_characters;
    QMap<QString, GameAction *> m_actions;
    QMap<QString, GameOption *> m_options;
    QMap<QString, GameObject *> m_objects;
    QMap<QString, int> m_objectCounts;

    // Mappings for registry resolving
    QMap<AH::Common::FieldData::FieldID, QStringList> m_fieldOptionMap;

    // Mappings for decks???
    QMap<AH::Common::FieldData::FieldID, QList<ArkhamEncounter *> > m_arkEnc;

    //typedef QMap<QString, Player *> PlayerMap;
    //PlayerMap m_player;
    //PlayerMap::iterator m_firstPlayer;
    QMap<QString, Player *> m_player;
    QList<Player *> m_playerList;
    QVector<GamePhase *> m_phases;
    static Game *s_instance;
    GameContext m_context;

    GameBoard *m_board;

    GameNotifier *m_notifier;


    int m_nextPlayerId;
    bool m_started;

    mutable QReadWriteLock m_lock;
};

#define gGame (Game::instance())

#endif // GAME_H
