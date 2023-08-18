#include "gameserver.h"
#include "ahglobal.h"
#include "clientconnection.h"
#include <utils/cleanupthread.h>
#include <QDebug>

GameServer::GameServer() = default;

void GameServer::start()
{
    AH::registerCommonMetaTypes();
    bool ok = listen(QHostAddress::Any, ServerPort);
    if (!ok) {
        qCritical() << QString("Cannot listen on TCP server port %1").arg(ServerPort);
    }
}

void GameServer::stop()
{
    this->close();
}

void GameServer::incomingConnection(qintptr socketDescriptor)
{
    AH::Common::NetworkConnection *conn = new AH::Common::NetworkConnection(socketDescriptor);
    ClientConnection *c = new ClientConnection(conn);
    conn->setParent(c);
    AH::Common::CleanupThread *t = new AH::Common::CleanupThread;
    c->moveToThread(t);
    connect(t, SIGNAL(started()), c, SLOT(startup()));
    connect(t, SIGNAL(finished()), c, SLOT(deleteLater()));
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(c, SIGNAL(shutdown()), t, SLOT(quit()));
    t->registerCleanupMethod(c, "cleanup");
    t->start();
}
