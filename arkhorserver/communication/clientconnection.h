#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <communication/networkconnection.h>

class NetworkPlayer;

class ClientConnection : public AH::Common::NetworkConnection
{
public:
    ClientConnection(qintptr socketDescriptor);

protected slots:
    void cleanup();

protected:
    void receivedMessage(const AH::Common::Message &msg);

private:
    void handleVersion(const QVariant &v);
    void handleRegisterPlayer();
    void handleStartGame();
    void handleRequestObjects(const QVariant &reqsData);

private:
    NetworkPlayer *m_player;
    bool m_versionReceived;
};

#endif // CLIENTCONNECTION_H
