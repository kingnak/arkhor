#include "connectionhandler.h"
#include <QThread>
#include <QDebug>
#include <playerdata.h>
#include <investigatordata.h>

ConnectionHandler::ConnectionHandler(QString host, int port)
    : m_host(host), m_port(port), m_conn(NULL)
{
}

void ConnectionHandler::confirmActive()
{
    send(AH::Common::Message::C_CONFIRM_ACTIVE);
}

void ConnectionHandler::registerPlayer()
{
    send(AH::Common::Message::C_REGISTER_PLAYER);
}

void ConnectionHandler::startGame()
{
    send(AH::Common::Message::C_START_GAME);
}

void ConnectionHandler::chooseInvestigator(AH::Common::InvestigatorData i)
{
    send(AH::Common::Message::C_SELECT_INVESTIGATOR, i.id());
}

void ConnectionHandler::selectOption(QString id)
{
    send(AH::Common::Message::C_SELECT_OPTION, id);
}

void ConnectionHandler::selectMovementPath(QList<AH::Common::FieldData::FieldID> fieldIds)
{
    QVariant v;
    v << fieldIds;
    send(AH::Common::Message::C_MOVE_PATH, v);
}

void ConnectionHandler::selectFocus(QList<int> positionDiffs)
{
    QVariant v;
    v << positionDiffs;
    send(AH::Common::Message::C_SELECT_FOCUS, v);
}

void ConnectionHandler::startup()
{
    QTcpSocket *sock = new QTcpSocket;
    m_conn = new AH::Common::NetworkConnection(sock);
    connect(sock, SIGNAL(connected()), this, SLOT(established()));
    connect(sock, SIGNAL(disconnected()), this, SLOT(sockError()));
    sock->connectToHost(m_host, m_port);
}

void ConnectionHandler::cleanup()
{
    if (m_conn) m_conn->cleanup();
    delete m_conn;
}

void ConnectionHandler::handleMessage(AH::Common::Message msg)
{
    switch (msg.type) {
    case AH::Common::Message::S_PROMPT_ACTIVE:
        emit promptActive();
        break;

    case AH::Common::Message::S_REGISTER_PLAYER_SUCCESSFUL:
    {
        AH::Common::PlayerData d;
        msg.payload >> d;
        emit setPlayerData(d);
        break;
    }
    case AH::Common::Message::S_REGISTER_PLAYER_FAILED:
        m_conn->stop();
        break;

    case AH::Common::Message::S_GAME_STARTED:
        emit gameStarted();
        break;

    case AH::Common::Message::S_CHOOSE_INVESTIGATOR:
    {
        QList<AH::Common::InvestigatorData> l;
        msg.payload >> l;
        emit setInvestigatorList(l);
        break;
    }

    case AH::Common::Message::S_PLAYER_CHAR_INSTANTIATED:
    {
        QVariantMap m;
        msg.payload >> m;
        emit playerCharacterInstantiated(m["player"].toString(), m["character"].toString());
        break;
    }

    case AH::Common::Message::S_BOARD_CONTENT:
    {
        //QVariantMap m;
        //msg.payload >> m;
        emit boardContent(msg.payload.value<QVariantMap>());
        break;
    }

    case AH::Common::Message::S_GAME_START:
        emit gameStart();
        break;

    case AH::Common::Message::S_CHOOSE_OPTION:
    {
        QList<AH::Common::GameOptionData> l;
        msg.payload >> l;
        emit chooseOption(l);
        break;
    }

    case AH::Common::Message::S_CHOOSE_MOVEMENT_PATH:
    {
        QVariantMap m;
        msg.payload >> m;
        AH::Common::FieldData::FieldID sid;
        int mov;
        m["startId"] >> sid;
        m["movementPoints"] >> mov;
        emit chooseMovement(sid, mov);
        break;
    }

    case AH::Common::Message::S_CHOOSE_FOCUS:
    {
        QVariantMap m;
        msg.payload >> m;
        QList<AH::Common::AttributeSliderData> sld;
        int amnt;
        m["focusAmount"] >> amnt;
        m["focusSliders"] >> sld;
        emit chooseFocus(sld, amnt);
        break;
    }

    default:
        qDebug() << "Did not understand message type: " << msg.type;
    }
}

void ConnectionHandler::sockError()
{
    emit disconnected();
}

void ConnectionHandler::established()
{
    m_conn->startup();
    connect(m_conn, SIGNAL(messageReceived(AH::Common::Message)), this, SLOT(handleMessage(AH::Common::Message)));
    emit connected();
}

void ConnectionHandler::send(AH::Common::Message::Type type, QVariant data)
{
    if (QThread::currentThread() == this->thread()) {
        doSend(type, data);
    } else {
        QMetaObject::invokeMethod(this,
                "doSend",
                Qt::QueuedConnection,
                Q_ARG(AH::Common::Message::Type, type),
                Q_ARG(QVariant, data));
    }
}

void ConnectionHandler::doSend(AH::Common::Message::Type type, QVariant data)
{
    m_conn->sendMessage(type, data);
}
