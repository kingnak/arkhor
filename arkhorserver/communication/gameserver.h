#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QTcpServer>

class GameServer : public QTcpServer
{
public:
    GameServer();

    void start();

protected:
    virtual void incomingConnection ( int socketDescriptor );

private:
};

#endif // GAMESERVER_H
