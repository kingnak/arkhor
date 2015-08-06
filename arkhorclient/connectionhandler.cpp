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

void ConnectionHandler::selectSkill(AH::Common::PropertyValueData::Property prop)
{
    QVariant v;
    v << prop;
    send(AH::Common::Message::C_SELECT_SKILL, prop);
}

void ConnectionHandler::chooseDieRollUpdate(AH::Common::DieTestUpdateData upd)
{
    QVariant v;
    v << upd;
    send(AH::Common::Message::C_DIE_ROLL_UPDATE, v);
}

void ConnectionHandler::requestObjects(AH::Common::RequestObjectsData reqs)
{
    QVariant v;
    v << reqs;
    send(AH::Common::Message::C_REQUEST_OBJECTS, v);
}

void ConnectionHandler::cancelWeapons()
{
    send(AH::Common::Message::C_CANCEL_WEAPONS);
}

void ConnectionHandler::selectWeapons(QStringList weaponIds)
{
    QVariant v;
    v << weaponIds;
    send(AH::Common::Message::C_SELECT_WEAPONS, v);
}

void ConnectionHandler::selectEncounterOption(QString id)
{
    send(AH::Common::Message::C_SELECT_ENCOUNTER, id);
}

void ConnectionHandler::acknowledge()
{
    send(AH::Common::Message::C_ACKNOWLEDGED);
}

void ConnectionHandler::choiceSelected(AH::Common::ChoiceResponseData resp)
{
    send(AH::Common::Message::C_SELECT_CHOICE, resp);
}

void ConnectionHandler::choiceCanceled()
{
    send(AH::Common::Message::C_CANCEL_CHOICE);
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
    case AH::Common::Message::S_VERSION:
    {
        quint32 v;
        msg.payload >> v;
        if (v != AH::Common::Message::PROTOCOL_VERSION) {
            emit versionMismatch(AH::Common::Message::PROTOCOL_VERSION, v);
        }
        break;
    }

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

    case AH::Common::Message::S_SETTING_DATA:
    {
        AH::Common::GameSettingData s;
        msg.payload >> s;
        emit settingUpdate(s);
        break;
    }

    case AH::Common::Message::S_CHARACTER_DATA:
    {
        AH::Common::CharacterData c;
        msg.payload >> c;
        emit characterUpdate(c);
        break;
    }

    case AH::Common::Message::S_DESCRIBE_OBJECTS:
    {
        AH::Common::DescribeObjectsData d;
        msg.payload >> d;
        emit objectDescriptions(d);
        break;
    }

    case AH::Common::Message::S_INVALIDATE_OBJECTS:
    {
        QStringList lst;
        msg.payload >> lst;
        emit objectInvalidations(lst);
        break;
    }

    case AH::Common::Message::S_INVALIDATE_OBJECTS_BY_TYPE:
    {
        AH::Common::RequestObjectsData::ObjectType type;
        msg.payload >> type;
        emit objectTypeInvalidation(type);
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

    case AH::Common::Message::S_CHOOSE_SKILL:
    {
        QList<AH::Common::ModifiedPropertyValueData> opts;
        msg.payload >> opts;
        emit chooseSkill(opts);
        break;
    }

    case AH::Common::Message::S_DIE_ROLL_INFO:
    {
        AH::Common::DieRollTestData t;
        msg.payload >> t;
        emit dieRollInfo(t);
        break;
    }

    case AH::Common::Message::S_CHOOSE_WEAPONS:
    {
        QVariantMap m;
        msg.payload >> m;
        AH::Common::ModifiedPropertyValueData hands;
        QList<AH::Common::GameObjectData> weapons;
        m["hands"] >> hands;
        m["weapons"] >> weapons;
        emit chooseWeapons(weapons, hands);
        break;
    }

    case AH::Common::Message::S_CHOOSE_ENCOUNTER:
    {
        AH::Common::EncounterData a;
        msg.payload >> a;
        emit chooseEncounterOption(a);
        break;
    }

    case AH::Common::Message::S_ACKNOWLEDGE_MYTHOS:
    {
        AH::Common::MythosData m;
        msg.payload >> m;
        emit displayMythos(m);
        break;
    }

    case AH::Common::Message::S_ABORT_ACKNOWLEDGE:
    {
        emit finishMythos();
        break;
    }

    case AH::Common::Message::S_OFFER_CHOICE:
    {
        AH::Common::ChoiceData d;
        msg.payload >> d;
        emit offerChoice(d);
        break;
    }

    case AH::Common::Message::S_WON:
    {
        emit won(msg.payload.toString());
        break;
    }

    case AH::Common::Message::S_LOST:
    {
        emit lost(msg.payload.toString());
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

    send(AH::Common::Message::C_VERSION, QVariant(AH::Common::Message::PROTOCOL_VERSION));

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
