#ifndef NETWORKPLAYER_H
#define NETWORKPLAYER_H

#include "game/player.h"
#include <communication/message.h>
#include <QTimer>

class QEventLoop;

class ClientConnection;
class NotificationFormatter;

class NetworkPlayer : public QObject, public Player
{
    Q_OBJECT
public:
    NetworkPlayer();
    ~NetworkPlayer();

    void setConnection(ClientConnection *conn);
    void removedFromGame() override;


    void init(Game *game) override;

    void playerRemoved(Player *p) override;

    void playerCharacterInstantiated(Player *p) override;

    void gameStarted() override;
    void startGame() override;

    void sendBoard(GameBoard *board, AH::Common::GameBoardChangeData changes) override;
    void sendBoardDescription(GameBoard *board, QVariantMap descriptions) override;
    void sendCharacter(Character *c) override;
    void sendSetting(AH::Common::GameSettingData data) override;

    void nextRound() override;
    void gamePhaseChanged(AH::GamePhase phase) override;

    void firstPlayerChanged(const Player *player) override;
    void currentPlayerChanged(const Player *player) override;

    void actionStart(const GameAction *action, QString desc = QString::null) override;
    void actionUpdate(const GameAction *action, QString desc = QString::null) override;
    void actionFinish(const GameAction *action, QString desc = QString::null) override;
    void actionExecute(const GameAction *action, QString desc = QString::null) override;

    void objectsInvalidated(QStringList id) override;
    void objectTypeInvalidated(AH::Common::RequestObjectsData::ObjectType type) override;
    void setTempData(const QString &data) override;
    void clearTempData() override;

    bool acknowledgeMythos(const MythosCard *m, QObject *observer) override;
    bool acknowledgeMonsterMovement(Monster *m, QObject *observer) override;
    void abortAcknowledge() override;

    void notifySimple(const QString &str, Player *p, const QString &desc = QString::null) override;
    void notifyAlert(const QString &msg, Player *p, const QString &desc = QString::null) override;
    void notifySpecific(const QString &strThat, const QString &strOther, Player *that, const QString &desc = QString::null) override;
    void notifyWon(QString msg) override;
    void notifyLost(QString msg) override;
    void notifyDied(Player *p) override;

    ///////// INTERACTOR:

    AH::Common::DieTestUpdateData dieRollStart(const AH::Common::DieRollTestData &test) override;
    AH::Common::DieTestUpdateData dieRollUpdate(const AH::Common::DieRollTestData &test) override;
    void dieRollFinish(const AH::Common::DieRollTestData &test) override;

    Investigator *chooseInvestigator(QList<Investigator *> invs) override;

    bool chooseWeapons(QList<GameObject *> weapons, ModifiedPropertyValue hands, QStringList &selected) override;

    // Main Game
    GameOption *chooseOption(QList<GameOption *> options) override;
    QList<int> chooseFocus(QList<AttributeSlider> sliders, int totalFocus) override;
    MovementPath chooseMovement(GameField *start, int movement) override;

    QString chooseEncounterOption(AH::Common::EncounterData *enc) override;
    QString chooseMonster(QString desc, QList<AH::Common::MonsterData> monsters) override;

    //AH::Common::CostList choosePayment(const AH::Common::Cost &c) override;
    AH::Common::PropertyValueData::Property chooseSkill(QList<AH::Common::ModifiedPropertyValueData> options) override;

    AH::Common::ChoiceResponseData offerChoice(AH::Common::ChoiceData choice) override;

    AH::Common::TradeData offerTrade(AH::Common::TradeData trade) override;
    void tradeCanceled(const QString &name) override;

    // interface to network
    void handleMessage(const AH::Common::Message &msg);

    void destroy();

signals:
    void receivedWaitedMesage();

protected:
    //void timerEvent(QTimerEvent *timerEvent);

private slots:
    void promptActive();
    void killPlayer();

private:
    Q_INVOKABLE void doHandleMessage(const AH::Common::Message &msg);

    void sendText(const QString &txt);

    bool awaitResponse(AH::Common::Message &outMsg, const QList<AH::Common::Message::Type> &acceptTypes);

private:
    Game *m_game;
    ClientConnection *m_conn;

    QList<AH::Common::Message::Type> m_waitMsgTypes;
    AH::Common::Message m_waitMsg;
    bool m_bWaitSuccessful;

    QTimer *m_promptTimer;
    QTimer *m_killTimer;

    QObject *m_ackReceiver;

    NotificationFormatter *m_formatter;

    static const int PROMPT_TIMEOUT = 60*1000;
    static const int KILL_TIMEOUT = 15*1000;
};

#endif // NETWORKPLAYER_H
