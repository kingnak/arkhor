#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QTcpServer>

class GameServer : public QTcpServer
{
    Q_OBJECT
public:
    GameServer();
    static constexpr quint16 ServerPort = 6572;

public slots:
    void start();
    void stop();


protected:
    void incomingConnection ( qintptr socketDescriptor ) override;

private:
};

#endif // GAMESERVER_H
