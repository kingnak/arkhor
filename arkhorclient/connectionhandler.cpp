#include "connectionhandler.h"
#include <QThread>
#include <QDebug>
#include <playerdata.h>
#include <investigatordata.h>

ConnectionHandler::ConnectionHandler(QString host, int port, int ct)
    : m_host(host)
    , m_port(port)
    //, m_conn(NULL)
    , m_ct(ct)
{
}

#define S_RET(ARG) {m_ret = m_rc.pop(); \
    ARG \
    m_ret = nullptr; \
    }
#define S_MAIN(ARG) {m_ret = m_mc; \
    ARG \
    m_ret = nullptr; \
    }
#define S_ALL(ARG) for (auto c_ : m_conns) {m_ret = c_; \
    ARG \
    } m_ret = nullptr;
void ConnectionHandler::confirmActive()
{
    S_RET(
                send(AH::Common::Message::C_CONFIRM_ACTIVE);
            )
}

bool isReg = false;
void ConnectionHandler::registerPlayer()
{
    if (isReg) return;
    isReg = true;
    S_ALL(
                send(AH::Common::Message::C_REGISTER_PLAYER);
            )
}

void ConnectionHandler::startGame()
{
    S_MAIN(send(AH::Common::Message::C_START_GAME);)
}

void ConnectionHandler::chooseInvestigator(AH::Common::InvestigatorData i)
{
    S_RET(send(AH::Common::Message::C_SELECT_INVESTIGATOR, i.id());)
}

void ConnectionHandler::selectOption(QString id)
{
    S_RET(send(AH::Common::Message::C_SELECT_OPTION, id);)
}

void ConnectionHandler::selectMovementPath(QList<AH::Common::FieldData::FieldID> fieldIds)
{
    QVariant v;
    v << fieldIds;
    S_RET(send(AH::Common::Message::C_MOVE_PATH, v);)
}

void ConnectionHandler::selectFocus(QList<int> positionDiffs)
{
    QVariant v;
    v << positionDiffs;
    S_RET(send(AH::Common::Message::C_SELECT_FOCUS, v);)
}

void ConnectionHandler::selectSkill(AH::Common::PropertyValueData::Property prop)
{
    QVariant v;
    v << prop;
    S_RET(send(AH::Common::Message::C_SELECT_SKILL, prop);)
}

void ConnectionHandler::chooseDieRollUpdate(AH::Common::DieTestUpdateData upd)
{
    QVariant v;
    v << upd;
    S_RET(send(AH::Common::Message::C_DIE_ROLL_UPDATE, v);)
}

void ConnectionHandler::requestObjects(AH::Common::RequestObjectsData reqs)
{
    QVariant v;
    v << reqs;
    S_MAIN(send(AH::Common::Message::C_REQUEST_OBJECTS, v);)
}

void ConnectionHandler::cancelWeapons()
{
    S_RET(send(AH::Common::Message::C_CANCEL_WEAPONS);)
}

void ConnectionHandler::selectWeapons(QStringList weaponIds)
{
    QVariant v;
    v << weaponIds;
    S_RET(send(AH::Common::Message::C_SELECT_WEAPONS, v);)
}

void ConnectionHandler::selectEncounterOption(QString id)
{
    S_RET(send(AH::Common::Message::C_SELECT_ENCOUNTER, id);)
}

void ConnectionHandler::selectMonster(QString id)
{
    S_RET(send(AH::Common::Message::C_SELECT_MONSTER, id);)
}

void ConnectionHandler::acknowledge()
{
    // TODO: Only over main
    S_ALL(send(AH::Common::Message::C_ACKNOWLEDGED);)
}

void ConnectionHandler::choiceSelected(AH::Common::ChoiceResponseData resp)
{
    S_RET(send(AH::Common::Message::C_SELECT_CHOICE, resp);)
}

void ConnectionHandler::choiceCanceled()
{
    S_RET(send(AH::Common::Message::C_CANCEL_CHOICE);)
}

void ConnectionHandler::tradeSelected(AH::Common::TradeData trade)
{
    QVariant v;
    v << trade;
    S_RET(send(AH::Common::Message::C_TRADE, v);)
}

void ConnectionHandler::tradeCanceled()
{
    S_RET(send(AH::Common::Message::C_CANCEL_TRADE);)
}

void ConnectionHandler::setSkipOption(AH::Common::PlayerData::AutoSkipData skipOption)
{
    S_ALL(send(AH::Common::Message::C_SET_AUTOSKIP, skipOption);)
}

void ConnectionHandler::startup()
{
    for (int i = 0; i < m_ct; ++i) {
        QTcpSocket *sock = new QTcpSocket;
        m_conns[sock] = new AH::Common::NetworkConnection(sock);
        if (i == 0) m_mc = m_conns[sock];
        connect(sock, SIGNAL(connected()), this, SLOT(established()));
        connect(sock, SIGNAL(disconnected()), this, SLOT(sockError()));
        sock->connectToHost(m_host, m_port);
    }
}
#undef S_ALL
#undef S_MAIN
#undef S_RET

void ConnectionHandler::cleanup()
{
    /*
    if (m_conn) m_conn->cleanup();
    delete m_conn;
    */
    for (auto c : m_conns) c->cleanup();
    qDeleteAll(m_conns);
}

#define M_SINGLE(ARG) {\
    ARG \
    }
#define M_MAIN(ARG) if (static_cast<AH::Common::NetworkConnection*>(sender()) == m_mc) {\
    ARG \
    }
#define M_ALL(ARG) {for (auto c_ : m_conns) \
    ARG \
    }
#define M_RET(ARG) {auto c_ = static_cast<AH::Common::NetworkConnection*>(sender()); m_rc<<c_;\
    ARG \
    }

void ConnectionHandler::handleMessage(AH::Common::Message msg)
{
    switch (msg.type) {
    case AH::Common::Message::S_VERSION:
    {
        M_MAIN(
        quint32 v;
        msg.payload >> v;
        if (v != AH::Common::Message::PROTOCOL_VERSION) {
            emit versionMismatch(AH::Common::Message::PROTOCOL_VERSION, v);
        }
        )
        break;
    }

    case AH::Common::Message::S_TEXT_MESSAGE:
    {
        M_MAIN(
        QString d;
        msg.payload >> d;
        emit textMessage(d);
        )
        break;
    }

    case AH::Common::Message::S_PROMPT_ACTIVE:
        /*M_RET(
        emit promptActive();
        break;
        );*/
        static_cast<AH::Common::NetworkConnection*>(sender())->sendMessage(AH::Common::Message::C_CONFIRM_ACTIVE, {});
        break;

    case AH::Common::Message::S_REGISTER_PLAYER_SUCCESSFUL:
    {
        M_SINGLE(
        AH::Common::PlayerData d;
        msg.payload >> d;
        m_thisPlayers[d.id()] = d;
        emit setPlayerData(d);
        )
        break;
    }
    case AH::Common::Message::S_REGISTER_PLAYER_FAILED:
        M_ALL(
        c_->stop();
        )
        break;

    case AH::Common::Message::S_GAME_STARTED:
        M_MAIN(
        emit gameStarted();
        )
        break;

    case AH::Common::Message::S_CHOOSE_INVESTIGATOR:
    {
        M_RET(
        QList<AH::Common::InvestigatorData> l;
        msg.payload >> l;
        emit setInvestigatorList(l);
        )
        break;
    }

    case AH::Common::Message::S_PLAYER_CHAR_INSTANTIATED:
    {
        M_SINGLE(
        QVariantMap m;
        msg.payload >> m;
        m_thisChars[m["player"].toString()] = m["character"].toString();
        emit playerCharacterInstantiated(m["player"].toString(), m["character"].toString());
        )
        break;
    }

    case AH::Common::Message::S_BOARD_CONTENT:
    {
        //QVariantMap m;
        //msg.payload >> m;
        M_MAIN(
        emit boardContent(msg.payload.value<QVariantMap>());
        )
        break;
    }

    case AH::Common::Message::S_BOARD_DESCRIPTION:
    {
        QVariantMap m, b, d;
        msg.payload >> m;
        m["board"] >> b;
        m["descriptions"] >> d;
        M_MAIN(
        emit boardDescription(b, d);
        )
        break;
    }

    case AH::Common::Message::S_SETTING_DATA:
    {
        M_MAIN(
        AH::Common::GameSettingData s;
        msg.payload >> s;
        emit settingUpdate(s);
        )
        break;
    }

    case AH::Common::Message::S_CHARACTER_DATA:
    {
        M_MAIN(
        AH::Common::CharacterData c;
        msg.payload >> c;
        emit characterUpdate(c);
        )
        break;
    }

    case AH::Common::Message::S_DESCRIBE_OBJECTS:
    {
        M_MAIN(
        AH::Common::DescribeObjectsData d;
        msg.payload >> d;
        emit objectDescriptions(d);
        )
        break;
    }

    case AH::Common::Message::S_INVALIDATE_OBJECTS:
    {
        M_MAIN(
        QStringList lst;
        msg.payload >> lst;
        emit objectInvalidations(lst);
        )
        break;
    }

    case AH::Common::Message::S_INVALIDATE_OBJECTS_BY_TYPE:
    {
        M_MAIN(
        AH::Common::RequestObjectsData::ObjectType type;
        msg.payload >> type;
        emit objectTypeInvalidation(type);
        )
        break;
    }

    case AH::Common::Message::S_GAME_START:
        M_MAIN(
        emit gameStart();
        )
        break;
    case AH::Common::Message::S_CHOOSE_OPTION:
    {
        M_RET(
        QList<AH::Common::GameOptionData> l;
        msg.payload >> l;
        emit chooseOption(l);
        )
        break;
    }

    case AH::Common::Message::S_CHOOSE_MOVEMENT_PATH:
    {
        M_RET(
        QVariantMap m;
        msg.payload >> m;
        AH::Common::FieldData::FieldID sid;
        int mov;
        m["startId"] >> sid;
        m["movementPoints"] >> mov;
        emit chooseMovement(sid, mov);
        )
        break;
    }

    case AH::Common::Message::S_CHOOSE_FOCUS:
    {
        M_RET(
        QVariantMap m;
        msg.payload >> m;
        QList<AH::Common::AttributeSliderData> sld;
        int amnt;
        m["focusAmount"] >> amnt;
        m["focusSliders"] >> sld;
        emit chooseFocus(sld, amnt);
        )
        break;
    }

    case AH::Common::Message::S_CHOOSE_SKILL:
    {
        M_RET(
        QList<AH::Common::ModifiedPropertyValueData> opts;
        msg.payload >> opts;
        emit chooseSkill(opts);
        )
        break;
    }

    case AH::Common::Message::S_DIE_ROLL_INFO:
    {
        M_RET(
        AH::Common::DieRollTestData t;
        msg.payload >> t;
        emit dieRollInfo(t);
        )
        break;
    }

    case AH::Common::Message::S_CHOOSE_WEAPONS:
    {
        M_RET(
        QVariantMap m;
        msg.payload >> m;
        AH::Common::ModifiedPropertyValueData hands;
        QList<AH::Common::GameObjectData> weapons;
        m["hands"] >> hands;
        m["weapons"] >> weapons;
        emit chooseWeapons(weapons, hands);
        )
        break;
    }

    case AH::Common::Message::S_CHOOSE_ENCOUNTER:
    {
        M_RET(
        AH::Common::EncounterData a;
        msg.payload >> a;
        emit chooseEncounterOption(a);
        )
        break;
    }

    case AH::Common::Message::S_CHOOSE_MONSTER:
    {
        M_RET(
        QString desc;
        QList<AH::Common::MonsterData> m;
        QVariantMap map;
        msg.payload >> map;
        map["description"] >> desc;
        map["monsters"] >> m;
        emit chooseMonster(desc, m);
        )
        break;
    }

    case AH::Common::Message::S_ACKNOWLEDGE_MYTHOS:
    {
        M_MAIN(
        AH::Common::MythosData m;
        msg.payload >> m;
        emit acknowledgeMythos(m);
        )
        break;
    }

    case AH::Common::Message::S_ACKNOWLEDGE_MONSTER_MOVEMENT:
    {
        M_MAIN(
        AH::Common::MonsterData m;
        msg.payload >> m;
        emit acknowledgeMonsterMovement(m);
        )
        break;
    }

    case AH::Common::Message::S_ABORT_ACKNOWLEDGE:
    {
        M_ALL(
        emit finishAcknowledge();
        )
        break;
    }

    case AH::Common::Message::S_OFFER_CHOICE:
    {
        M_RET(
        AH::Common::ChoiceData d;
        msg.payload >> d;
        emit offerChoice(d);
        )
        break;
    }

    case AH::Common::Message::S_SET_TEMP_DATA:
    {
        M_MAIN(
        QString d;
        msg.payload >> d;
        emit setTempData(d);
        )
        break;
    }

    case AH::Common::Message::S_CLEAR_TEMP_DATA:
    {
        M_MAIN(
        emit clearTempData();
        )
        break;
    }

    case AH::Common::Message::S_DIED:
    {
        M_MAIN(
        emit died(msg.payload.toString());
        )
        break;
    }

    case AH::Common::Message::S_WON:
    {
        M_MAIN(
        emit won(msg.payload.toString());
        )
        break;
    }

    case AH::Common::Message::S_LOST:
    {
        M_MAIN(
        emit lost(msg.payload.toString());
        )
        break;
    }

    case AH::Common::Message::S_GAME_ALERT:
    {
        // TODO: auto-reply
        M_MAIN(
        emit gameAlert(msg.payload.toString());
        )
        break;
    }

    case AH::Common::Message::S_PHASE_CHANGE:
    {
        M_MAIN(
        AH::GamePhase ph;
        msg.payload >> ph;
        emit phaseChange(ph);
        )
        break;
    }

    case AH::Common::Message::S_PLAYER_CHANGE:
    {
        M_MAIN(
        QString id;
        msg.payload >> id;

        if (m_thisPlayers.contains(id)) {
            //emit setPlayerData(m_thisPlayers[id]);
            //emit playerCharacterInstantiated(id, m_thisChars[id]);
            emit overridePlayerId(id);
            emit overrideCharacterId(m_thisChars[id]);
        }

        emit playerChange(id);
        )
        break;

    case AH::Common::Message::S_TRADE:
    {
        M_RET(
        AH::Common::TradeData td;
        msg.payload >> td;
        emit offerTrade(td);
        )
        break;
    }

    case AH::Common::Message::S_CANCEL_TRADE:
        M_RET(
        QString name = msg.payload.toString();
        emit canceledTrade(name);
        )
        break;
    }

    default:
        qDebug() << "Did not understand message type: " << msg.type;
    }

#undef M_SINGLE
#undef M_RET
#undef M_ALL
}

void ConnectionHandler::sockError()
{
    emit disconnected();
}

void ConnectionHandler::established()
{
    AH::Common::NetworkConnection *c = m_conns[(QTcpSocket*)sender()];
    c->startup();
    connect(c, SIGNAL(messageReceived(AH::Common::Message)), this, SLOT(handleMessage(AH::Common::Message)));

    rsend(c, AH::Common::Message::C_VERSION, QVariant(AH::Common::Message::PROTOCOL_VERSION));

    emit connected();
}


void ConnectionHandler::send(AH::Common::Message::Type type, QVariant data)
{
    rsend(m_ret, type, data);
}

void ConnectionHandler::rsend(AH::Common::NetworkConnection *c, AH::Common::Message::Type type, QVariant data)
{
    if (QThread::currentThread() == this->thread()) {
        doSend(c, type, data);
    } else {
        QMetaObject::invokeMethod(this,
                "doSend",
                Qt::QueuedConnection,
                Q_ARG(AH::Common::NetworkConnection*, c),
                Q_ARG(AH::Common::Message::Type, type),
                Q_ARG(QVariant, data));
    }
}

void ConnectionHandler::doSend(AH::Common::NetworkConnection *c, AH::Common::Message::Type type, QVariant data)
{
    c->sendMessage(type, data);
}
