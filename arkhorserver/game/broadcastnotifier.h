#ifndef BROADCASTNOTIFIER_H
#define BROADCASTNOTIFIER_H

#include "game/gamenotifier.h"
#include <QSet>
#include <QEventLoop>

class BroadcastNotifier : public QObject, public GameNotifier
{
    Q_OBJECT
public:
    BroadcastNotifier();

    void init(Game *game) override;

    void gameStarted() override;
    void startGame() override;

    void sendBoard(GameBoard *board, AH::Common::GameBoardChangeData changes) override;
    void sendBoardDescription(GameBoard *board, QVariantMap descriptions) override;
    void sendCharacter(Character *c) override;
    void sendSetting(AH::Common::GameSettingData data) override;

    void playerRemoved(Player *p) override;

    void nextRound() override;
    void gamePhaseChanged(AH::GamePhase phase) override;

    void firstPlayerChanged(const Player *player) override;
    void currentPlayerChanged(const Player *player) override;

    void actionStart(const GameAction *action, QString desc = QString::null) override;
    void actionUpdate(const GameAction *action, QString desc = QString::null) override;
    void actionFinish(const GameAction *action, QString desc = QString::null) override;
    void actionExecute(const GameAction *action, QString desc = QString::null) override;

    void objectsInvalidated(QStringList ids) override;
    void objectTypeInvalidated(AH::Common::RequestObjectsData::ObjectType type) override;
    void setTempData(const QString &data) override;
    void clearTempData() override;

    bool acknowledgeMythos(const MythosCard *m, QObject *observer = NULL) override;
    bool acknowledgeMonsterMovement(Monster *m, QObject *observer = NULL) override;
    void abortAcknowledge() override;

    void notifySimple(const QString &str, Player *p, const QString &desc = QString::null) override;
    void notifyAlert(const QString &msg, Player *p, const QString &desc = QString::null) override;
    void notifySpecific(const QString &strThat, const QString &strOther, Player *that, const QString &desc = QString::null) override;
    void notifyWon(QString msg) override;
    void notifyLost(QString msg) override;
    void notifyDied(Player *p) override;

    bool event(QEvent *eve) override;

private:
    bool doAcknowledge(std::function<bool(GameNotifier *)> func);

private:
    Game *m_game;
    QSet<Player *> m_openAcks;
    QEventLoop m_ackLoop;
    int m_ackTimeoutId;
};


#endif // BROADCASTNOTIFIER_H
