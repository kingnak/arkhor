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

    virtual void init(Game *game);

    virtual void gameStarted();
    virtual void startGame();

    virtual void sendBoard(GameBoard *board);
    virtual void sendCharacter(Character *c);
    virtual void sendSetting(AH::Common::GameSettingData data);

    virtual void playerRemoved(Player *p);

    virtual void nextRound();
    virtual void gamePhaseChaned(AH::GamePhase phase);

    virtual void firstPlayerChanged(const Player *player);
    virtual void currentPlayerChanged(const Player *player);

    virtual void actionStart(const GameAction *action, QString desc = QString::null);
    virtual void actionUpdate(const GameAction *action, QString desc = QString::null);
    virtual void actionFinish(const GameAction *action, QString desc = QString::null);
    virtual void actionExecute(const GameAction *action, QString desc = QString::null);

    virtual void objectsInvalidated(QStringList ids);
    virtual void objectTypeInvalidated(AH::Common::RequestObjectsData::ObjectType type);

    virtual bool acknowledgeMythos(const MythosCard *m, QObject *observer = NULL);
    virtual void abortAcknowledge();

    void notifyWon(QString msg);
    void notifyLost(QString msg);
    void notifyDied(Player *p);

    virtual bool event(QEvent *eve);

private:
    Game *m_game;
    QSet<Player *> m_openAcks;
    QEventLoop m_ackLoop;
    int m_ackTimeoutId;
};


#endif // BROADCASTNOTIFIER_H
