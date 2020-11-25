#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <communication/networkconnection.h>
#include <playerdata.h>
#include <investigatordata.h>
#include <gameoptiondata.h>
#include <attributesliderdata.h>
#include <diedata.h>
#include <characterdata.h>
#include <gameobjectdata.h>
#include <objectdata.h>
#include <encounterdata.h>
#include <mythosdata.h>
#include <choicedata.h>
#include <gamesettingdata.h>
#include <monsterdata.h>
#include <tradedata.h>
#include <QStack>

class QTcpSocket;

class ConnectionHandler : public QObject
{
    Q_OBJECT
public:
    ConnectionHandler(const QString &host, int port, int ct);

    void confirmActive();
    void registerPlayer();
    void startGame();
    void chooseInvestigator(const AH::Common::InvestigatorData &i);
    void selectOption(const QString &id);
    void selectMovementPath(const QList<AH::Common::FieldData::FieldID> &fieldIds);
    void selectFocus(const QList<int> &positionDiffs);
    void selectSkill(AH::Common::PropertyValueData::Property prop);
    void chooseDieRollUpdate(const AH::Common::DieTestUpdateData &upd);

    void requestObjects(const AH::Common::RequestObjectsData &reqs);

    void cancelWeapons();
    void selectWeapons(const QStringList &weaponIds);

    void selectEncounterOption(const QString &id);

    void selectMonster(const QString &id);

    void acknowledge();

    void choiceSelected(const AH::Common::ChoiceResponseData &resp);
    void choiceCanceled();

    void tradeSelected(const AH::Common::TradeData &trade);
    void tradeCanceled();

    void setSkipOption(AH::Common::PlayerData::AutoSkipData skipOption);

public slots:
    void startup();
    void cleanup();

signals:
    void connected();
    void disconnected();

    void overridePlayerId(QString id);
    void overrideCharacterId(QString id);

    void versionMismatch(quint32 thisVersion, quint32 serverVersion);
    void textMessage(const QString &text);

    void promptActive();

    void gameStarted();
    void gameStart();
    void setPlayerData(AH::Common::PlayerData d);
    void setInvestigatorList(QList<AH::Common::InvestigatorData> l);

    void chooseOption(QList<AH::Common::GameOptionData> o);
    void chooseMovement(AH::Common::FieldData::FieldID startId, int movementPoints);
    void chooseFocus(QList<AH::Common::AttributeSliderData> sliders, int focusAmount);
    void chooseSkill(QList<AH::Common::ModifiedPropertyValueData> opts);
    void chooseWeapons(QList<AH::Common::GameObjectData> weapons, AH::Common::ModifiedPropertyValueData hands);
    void chooseEncounterOption(AH::Common::EncounterData encounter);
    void chooseMonster(QString desc, QList<AH::Common::MonsterData> monsters);

    void dieRollInfo(AH::Common::DieRollTestData data);

    void boardContent(QVariantMap boardMap);
    void boardDescription(QVariantMap boardMap, QVariantMap boardDesc);
    void characterUpdate(AH::Common::CharacterData c);
    void settingUpdate(AH::Common::GameSettingData d);

    void objectDescriptions(AH::Common::DescribeObjectsData descs);
    void objectInvalidations(QStringList lst);
    void objectTypeInvalidation(AH::Common::RequestObjectsData::ObjectType type);
    void setTempData(const QString &data);
    void clearTempData();

    void playerCharacterInstantiated(QString playerId, QString investigatorId);

    void acknowledgeMythos(AH::Common::MythosData mythos);
    void acknowledgeMonsterMovement(AH::Common::MonsterData monster);
    void finishAcknowledge();

    void offerChoice(AH::Common::ChoiceData choice);
    void offerTrade(AH::Common::TradeData trade);
    void canceledTrade(QString name);

    void died(QString msg);
    void won(QString msg);
    void lost(QString msg);
    void gameAlert(QString msg);
    void phaseChange(AH::GamePhase ph);
    void playerChange(QString playerId);

private slots:
    void handleMessage(const AH::Common::Message &msg);
    void sockError();
    void established();
    void rsend(AH::Common::NetworkConnection *c, AH::Common::Message::Type type, const QVariant &data = QVariant());
    void send(AH::Common::Message::Type type, const QVariant &data = QVariant());

private:
    Q_INVOKABLE void doSend(AH::Common::NetworkConnection *c, AH::Common::Message::Type type, const QVariant &data);

private:
    QString m_host;
    int m_port;

    int m_ct;
    QMap<QTcpSocket*, AH::Common::NetworkConnection *> m_conns;
    AH::Common::NetworkConnection *m_mc = nullptr;
    QStack<AH::Common::NetworkConnection*> m_rc;
    AH::Common::NetworkConnection *m_ret = nullptr;

    QMap<QString, AH::Common::PlayerData> m_thisPlayers;
    QMap<QString, QString> m_thisChars;
};

#endif // CONNECTIONHANDLER_H
