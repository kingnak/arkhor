#include "clientconnection.h"
#include "communication/networkplayer.h"
#include "game/game.h"
#include <objectdata.h>

using namespace AH::Common;

ClientConnection::ClientConnection(qintptr socketDescriptor)
:   NetworkConnection(socketDescriptor),
    m_player(NULL),
    m_versionReceived(false)
{
    setPinging();
}

void ClientConnection::cleanup()
{
    if (m_player) {
        m_player->destroy();
    }
    NetworkConnection::cleanup();
}

void ClientConnection::receivedMessage(const Message &msg)
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
        handleRequestObjects(msg.payload);
        break;

    default:
        if (m_player) {
            m_player->handleMessage(msg);
        } else {
            // ???
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
        m_player = NULL;

        sendMessage(Message::S_REGISTER_PLAYER_FAILED, "Error in register");
    }
}

void ClientConnection::handleStartGame()
{
    gGame->invoke("start");
}

void ClientConnection::handleRequestObjects(const QVariant &reqsData)
{
    RequestObjectsData reqs;
    reqsData >> reqs;

    DescribeObjectsData descs = gGame->describeObjects(reqs);
    QVariant v;
    v << descs;
    sendMessage(Message::S_DESCRIBE_OBJECTS, v);
}
