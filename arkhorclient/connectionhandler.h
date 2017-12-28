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

class ConnectionHandler : public QObject
{
    Q_OBJECT
public:
    ConnectionHandler(QString host, int port);

    void confirmActive();
    void registerPlayer();
    void startGame();
    void chooseInvestigator(AH::Common::InvestigatorData i);
    void selectOption(QString id);
    void selectMovementPath(QList<AH::Common::FieldData::FieldID> fieldIds);
    void selectFocus(QList<int> positionDiffs);
    void selectSkill(AH::Common::PropertyValueData::Property prop);
    void chooseDieRollUpdate(AH::Common::DieTestUpdateData upd);

    void requestObjects(AH::Common::RequestObjectsData reqs);

    void cancelWeapons();
    void selectWeapons(QStringList weaponIds);

    void selectEncounterOption(QString id);

    void acknowledge();

    void choiceSelected(AH::Common::ChoiceResponseData resp);
    void choiceCanceled();

    void setSkipOption(AH::Common::PlayerData::AutoSkipData skipOption);

public slots:
    void startup();
    void cleanup();

signals:
    void connected();
    void disconnected();

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

    void dieRollInfo(AH::Common::DieRollTestData data);

    void boardContent(QVariantMap boardMap);
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

    void died(QString msg);
    void won(QString msg);
    void lost(QString msg);
    void gameAlert(QString msg);
	void phaseChange(AH::GamePhase ph);
    void playerChange(QString playerId);

private slots:
    void handleMessage(AH::Common::Message msg);
    void sockError();
    void established();
    void send(AH::Common::Message::Type type, QVariant data = QVariant());

private:
    Q_INVOKABLE void doSend(AH::Common::Message::Type type, QVariant data);

private:
    QString m_host;
    int m_port;

    AH::Common::NetworkConnection *m_conn;
};

#endif // CONNECTIONHANDLER_H
