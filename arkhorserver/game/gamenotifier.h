#ifndef GAMENOTIFIER_H
#define GAMENOTIFIER_H

#include <ahglobal.h>
#include <QList>
#include <QString>
#include <QEvent>
#include <gamesettingdata.h>
#include <objectdata.h>

class Player;
class Game;
class GameAction;
class GameOption;
class GameBoard;
class Character;
class MythosCard;
class Monster;

class GameNotifier
{
public:
    GameNotifier() {}
    virtual ~GameNotifier() {}

    virtual void init(Game *game) = 0;
    virtual void gameStarted() = 0;

    virtual void playerRemoved(Player *p) = 0;

    virtual void startGame() = 0;

    virtual void sendBoard(GameBoard *board) = 0;
    virtual void sendCharacter(Character *c) = 0;
    virtual void sendSetting(AH::Common::GameSettingData data) = 0;

    virtual void nextRound() = 0;
    virtual void gamePhaseChanged(AH::GamePhase phase) = 0;

    virtual void firstPlayerChanged(const Player *player) = 0;
    virtual void currentPlayerChanged(const Player *player) = 0;

    virtual void actionStart(const GameAction *action, QString desc = QString::null) = 0;
    virtual void actionUpdate(const GameAction *action, QString desc = QString::null) = 0;
    virtual void actionFinish(const GameAction *action, QString desc = QString::null) = 0;
    virtual void actionExecute(const GameAction *action, QString desc = QString::null) = 0;

    virtual void objectsInvalidated(QStringList id) = 0;
    virtual void objectTypeInvalidated(AH::Common::RequestObjectsData::ObjectType type) = 0;
    virtual void setTempData(const QString &data) = 0;
    virtual void clearTempData() = 0;

    //virtual bool acknowledgeText(const QString &text, const QString &sourceId, QObject *observer = NULL) = 0;
    virtual bool acknowledgeMythos(const MythosCard *m, QObject *observer = NULL) = 0;
    virtual bool acknowledgeMonsterMovement(Monster *m, QObject *observer = NULL) = 0;

    virtual void abortAcknowledge() = 0;

    virtual void notifySimple(const QString &str, Player *p, const QString &desc = QString::null) = 0;
    virtual void notifyAlert(const QString &msg, Player *p, const QString &desc = QString::null) = 0;
    virtual void notifySpecific(const QString &strThat, const QString &strOther, Player *that, const QString &desc = QString::null) = 0;
    virtual void notifyWon(QString msg) = 0;
    virtual void notifyLost(QString msg) = 0;
    virtual void notifyDied(Player *p) = 0;
};


class AcknowledgeEvent : public QEvent {
public:
    static QEvent::Type Type() { return s_type; }
    AcknowledgeEvent(Player *p) : QEvent(AcknowledgeEvent::Type()), m_player(p) {}
    Player *player() { return m_player; }

private:
    Player *m_player;
/*
    static QEvent::Type s_gettype() {
        static int generatedType = QEvent::registerEventType();
        return static_cast<QEvent::Type>(generatedType);
    }
*/
    static const QEvent::Type s_type;
};


#endif // GAMENOTIFIER_H
