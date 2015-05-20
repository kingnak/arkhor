#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <communication/networkconnection.h>

class NetworkPlayer;

class ClientConnection : public AH::Common::NetworkConnection
{
public:
    ClientConnection(int socketDescriptor);

protected slots:
    void cleanup();

protected:
    void receivedMessage(const AH::Common::Message &msg);

private:
    void handleRegisterPlayer();
    void handleStartGame();
    void handleRequestObjects(const QVariant &reqsData);

private:
    NetworkPlayer *m_player;
};

#endif // CLIENTCONNECTION_H
