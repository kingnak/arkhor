#include "clientconnection.h"
#include "communication/networkplayer.h"
#include "game/game.h"
#include <objectdata.h>

using namespace AH::Common;

ClientConnection::ClientConnection(INetworkConnection *conn)
:   m_conn(conn),
    m_player(nullptr),
    m_versionReceived(false)
{
#ifndef DEBUG_SCRIPT_BUILD
    m_conn->setPinging();
#endif

    connect(conn, &INetworkConnection::shutdown, this, &ClientConnection::shutdown);
    connect(conn, &INetworkConnection::messageReceived, this, &ClientConnection::handleMessage);
    connect(conn, &INetworkConnection::messageReceivedWithId, this, &ClientConnection::handleMessageWithId);
}

void ClientConnection::cleanup()
{
    if (m_player) {
        m_player->destroy();
    }
    m_conn->cleanup();
}

void ClientConnection::handleMessage(const Message &msg)
{
    handleMessageWithId(msg, -1);
}

void ClientConnection::handleMessageWithId(const Message &msg, intptr_t msgId)
{
    if (msg.type != Message::C_VERSION) {
        if (!m_versionReceived) {
            abort();
            return;
        }
    }

    switch (msg.type) {
    case Message::C_VERSION:
        handleVersion(msg.payload);
        break;

    case Message::C_REGISTER_PLAYER:
        handleRegisterPlayer();
        break;

    case Message::C_START_GAME:
        handleStartGame();
        break;

    case Message::C_REQUEST_OBJECTS:
        handleRequestObjects(msg.payload, msgId);
        break;

    default:
        if (m_player) {
            m_player->handleMessage(msg);
        } else {
            // ???
            qWarning() << "Client connection received unexpected message while not having player: " << Message::msg_to_str(msg.type);
        }
    }
}

void ClientConnection::handleVersion(const QVariant &v)
{
    quint32 vers;
    v >> vers;
    m_versionReceived = true;

    sendMessage(Message::S_VERSION, QVariant(Message::PROTOCOL_VERSION));
    if (vers != Message::PROTOCOL_VERSION) {
        close();
    }
}

void ClientConnection::handleRegisterPlayer()
{
    if (m_player) {
        sendMessage(Message::S_REGISTER_PLAYER_FAILED, "Player already registered");
        return;
    }

    m_player = new NetworkPlayer();
    if (gGame->addPlayer(m_player)) {
        m_player->moveToThread(gGame->thread());
        m_player->setConnection(this);
        QVariant v;
        v << *(m_player->data());
        sendMessage(Message::S_REGISTER_PLAYER_SUCCESSFUL, v);
    } else {
        delete m_player;
        m_player = nullptr;

        sendMessage(Message::S_REGISTER_PLAYER_FAILED, "Error in register");
    }
}

void ClientConnection::handleStartGame()
{
    gGame->invoke("start");
}

void ClientConnection::handleRequestObjects(const QVariant &reqsData, intptr_t msgId)
{
    RequestObjectsData reqs;
    reqsData >> reqs;

    DescribeObjectsData descs = gGame->describeObjects(reqs);
    QVariant v;
    v << descs;
    m_conn->sendMessage(Message::S_DESCRIBE_OBJECTS, v, msgId);
}

void ClientConnection::sendMessage(Message::Type type, QVariant payload)
{
    m_conn->sendMessage(type, payload);
}

void ClientConnection::close()
{
    m_conn->close();
}

void ClientConnection::stop()
{
    m_conn->stop();
}

void ClientConnection::startup()
{
    m_conn->startup();
}

void ClientConnection::flush()
{
    m_conn->flush();
}
