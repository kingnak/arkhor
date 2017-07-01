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
    bool ok = listen(QHostAddress::Any, 6572);
    if (!ok) {
        qFatal("Cannot listen on socket");
    }
}

void GameServer::incomingConnection(qintptr socketDescriptor)
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
