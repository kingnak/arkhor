#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <communication/networkconnection.h>

class NetworkPlayer;

class ClientConnection : public QObject
{
    Q_OBJECT
public:
    ClientConnection(AH::Common::INetworkConnection *conn);

public slots:
    void sendMessage(AH::Common::Message::Type, QVariant payload);
    void close();
    void stop();
    void startup();
    void flush();

protected slots:
    void cleanup();

private slots:
    void handleMessage(const AH::Common::Message &msg);
    void handleMessageWithId(const AH::Common::Message &msg, intptr_t msgId);

signals:
    void shutdown();

private:
    void handleVersion(const QVariant &v);
    void handleRegisterPlayer();
    void handleStartGame();
    void handleRequestObjects(const QVariant &reqsData, intptr_t msgId);



private:
    AH::Common::INetworkConnection *m_conn;
    NetworkPlayer *m_player;
    bool m_versionReceived;
};

#endif // CLIENTCONNECTION_H
