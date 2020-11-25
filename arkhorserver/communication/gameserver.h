#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QTcpServer>

class GameServer : public QTcpServer
{
public:
    GameServer();

    void start();

    static constexpr quint16 ServerPort = 6572;

protected:
    void incomingConnection ( qintptr socketDescriptor ) override;

private:
};

#endif // GAMESERVER_H
