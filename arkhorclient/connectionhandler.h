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
#include <objectdata.h>

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

public slots:
    void startup();
    void cleanup();

signals:
    void connected();
    void disconnected();

    void promptActive();

    void gameStarted();
    void gameStart();
    void setPlayerData(AH::Common::PlayerData d);
    void setInvestigatorList(QList<AH::Common::InvestigatorData> l);

    void chooseOption(QList<AH::Common::GameOptionData> o);
    void chooseMovement(AH::Common::FieldData::FieldID startId, int movementPoints);
    void chooseFocus(QList<AH::Common::AttributeSliderData> sliders, int focusAmount);
    void chooseSkill(QList<AH::Common::ModifiedPropertyValueData> opts);

    void dieRollInfo(AH::Common::DieRollTestData data);

    void boardContent(QVariantMap boardMap);
    void characterUpdate(AH::Common::CharacterData c);

    void objectDescriptions(AH::Common::DescribeObjectsData descs);

    void playerCharacterInstantiated(QString playerId, QString investigatorId);

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
