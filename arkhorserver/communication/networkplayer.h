#ifndef NETWORKPLAYER_H
#define NETWORKPLAYER_H

#include "game/player.h"
#include <communication/message.h>
#include <QTimer>

class QEventLoop;

class ClientConnection;

class NetworkPlayer : public QObject, public Player
{
    Q_OBJECT
public:
    NetworkPlayer();
    ~NetworkPlayer();

    void setConnection(ClientConnection *conn);
    virtual void removedFromGame();


    virtual void init(Game *game);

    virtual void playerRemoved(Player *p);

    virtual void playerCharacterInstantiated(Player *p);

    virtual void gameStarted();
    virtual void startGame();

    virtual void sendBoard(GameBoard *board);
    virtual void sendCharacter(Character *c);

    virtual void nextRound();
    virtual void gamePhaseChaned(AH::GamePhase phase);

    virtual void firstPlayerChanged(const Player *player);
    virtual void currentPlayerChanged(const Player *player);

    virtual void actionStart(const GameAction *action, QString desc = QString::null);
    virtual void actionUpdate(const GameAction *action, QString desc = QString::null);
    virtual void actionFinish(const GameAction *action, QString desc = QString::null);
    virtual void actionExecute(const GameAction *action, QString desc = QString::null);

    virtual void objectsInvalidated(QStringList id);

    virtual bool acknowledgeMythos(const MythosCard *m, QObject *observer);
    virtual void abortAcknowledge();

    ///////// INTERACTOR:

    virtual AH::Common::DieTestUpdateData dieRollStart(const AH::Common::DieRollTestData test);
    virtual AH::Common::DieTestUpdateData dieRollUpdate(const AH::Common::DieRollTestData test);
    virtual void dieRollFinish(const AH::Common::DieRollTestData test);

    virtual Investigator *chooseInvestigator(QList<Investigator *> invs);

    virtual bool chooseWeapons(QList<GameObject *> weapons, ModifiedPropertyValue hands, QStringList &selected);

    // Main Game
    virtual GameOption *chooseOption(QList<GameOption *> options);
    virtual QList<int> chooseFocus(QList<AttributeSlider> sliders, int totalFocus);
    virtual MovementPath chooseMovement(GameField *start, int movement);

    virtual QString chooseEncounterOption(AH::Common::EncounterData *enc);

    virtual AH::Common::CostList choosePayment(const AH::Common::Cost &c);
    virtual AH::Common::PropertyValueData::Property chooseSkill(QList<AH::Common::ModifiedPropertyValueData> options);

    // interface to network
    void handleMessage(AH::Common::Message msg);

    void destroy();

signals:
    void receivedWaitedMesage();

protected:
    //void timerEvent(QTimerEvent *timerEvent);

private slots:
    void promptActive();
    void killPlayer();

private:
    Q_INVOKABLE void doHandleMessage(AH::Common::Message msg);

    void sendText(QString txt);

    bool awaitResponse(AH::Common::Message &outMsg, QList<AH::Common::Message::Type> acceptTypes);

private:
    Game *m_game;
    ClientConnection *m_conn;

    QList<AH::Common::Message::Type> m_waitMsgTypes;
    AH::Common::Message m_waitMsg;
    bool m_bWaitSuccessful;

    QTimer *m_promptTimer;
    QTimer *m_killTimer;

    QObject *m_ackReceiver;

    static const int PROMPT_TIMEOUT = 60*1000;
    static const int KILL_TIMEOUT = 15*1000;
};

#endif // NETWORKPLAYER_H
