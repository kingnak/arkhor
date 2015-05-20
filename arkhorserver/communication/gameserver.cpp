#include "gameserver.h"
#include "ahglobal.h"
#include "clientconnection.h"
#include <utils/cleanupthread.h>

GameServer::GameServer()
{
}

void GameServer::start()
{
    AH::registerCommonMetaTypes();
    listen(QHostAddress::Any, 6572);
}

void GameServer::incomingConnection(int socketDescriptor)
{
    ClientConnection *c = new ClientConnection(socketDescriptor);
    AH::Common::CleanupThread *t = new AH::Common::CleanupThread;
    c->moveToThread(t);
    connect(t, SIGNAL(started()), c, SLOT(startup()));
    connect(t, SIGNAL(finished()), c, SLOT(deleteLater()));
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(c, SIGNAL(shutdown()), t, SLOT(quit()));
    t->registerCleanupMethod(c, "cleanup");
    t->start();
}
