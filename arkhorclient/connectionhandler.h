#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <communication/networkconnection.h>
#include <playerdata.h>
#include <investigatordata.h>

class ConnectionHandler : public QObject
{
    Q_OBJECT
public:
    ConnectionHandler(QString host, int port);

    void confirmActive();
    void registerPlayer();
    void startGame();
    void chooseInvestigator(AH::Common::InvestigatorData i);

public slots:
    void startup();
    void cleanup();

signals:
    void connected();
    void disconnected();

    void promptActive();

    void gameStarted();
    void setPlayerData(AH::Common::PlayerData d);
    void setInvestigatorList(QList<AH::Common::InvestigatorData> l);

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
