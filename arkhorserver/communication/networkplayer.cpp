#include <QCoreApplication>
#include "networkplayer.h"
#include "clientconnection.h"
#include "game/game.h"
#include "game/gameaction.h"
#include <cost.h>
#include "game/investigator.h"
#include "character.h"
#include "game/gameoption.h"
#include "game/gameobject.h"
#include "game/arkhamencounter.h"
#include "game/mythoscard.h"
#include "notificationformatter.h"
#include <QEventLoop>
#include <QThread>
#include <QTimerEvent>

using namespace AH::Common;

NetworkPlayer::NetworkPlayer()
    : m_game(nullptr)
    , m_conn(nullptr)
    , m_bWaitSuccessful(false)
    , m_ackReceiver(nullptr)
    , m_formatter(new NotificationFormatter)
{
    m_promptTimer = new QTimer(this);
    m_promptTimer->setSingleShot(true);
    m_promptTimer->setInterval(PROMPT_TIMEOUT);
    connect(m_promptTimer, SIGNAL(timeout()), this, SLOT(promptActive()));

    m_killTimer = new QTimer(this);
    m_killTimer->setSingleShot(true);
    m_killTimer->setInterval(KILL_TIMEOUT);
    connect(m_killTimer, SIGNAL(timeout()), this, SLOT(killPlayer()));
}

NetworkPlayer::~NetworkPlayer()
{
    delete m_formatter;
}

void NetworkPlayer::setConnection(ClientConnection *conn)
{
    m_conn = conn;
}

void NetworkPlayer::removedFromGame()
{
    if (m_conn) {
        m_conn->sendMessage(Message::S_KILL, QVariant());
        m_conn->stop();
    }
}

void NetworkPlayer::init(Game *game)
{
    m_game = game;
}

void NetworkPlayer::playerRemoved(Player *p)
{
    QVariant m;
    m << *p->data();
    m_conn->sendMessage(Message::S_PLAYER_REMOVED, m);
}

void NetworkPlayer::playerCharacterInstantiated(Player *p)
{
    QVariantMap m;
    m["player"] << p->id();
    m["character"] << p->getCharacter()->id();
    m_conn->sendMessage(Message::S_PLAYER_CHAR_INSTANTIATED, m);
}

void NetworkPlayer::gameStarted()
{
    m_conn->sendMessage(Message::S_GAME_STARTED, QVariant());
}

void NetworkPlayer::startGame()
{
    m_conn->sendMessage(Message::S_GAME_START, QVariant());
    m_conn->flush();
}

void NetworkPlayer::sendBoard(GameBoard *board, GameBoardChangeData changes)
{
    QVariantMap m;
    m["_changes"] << changes;
    for (auto f : board->allFields()) {
        m[QString::number(f->id())] << *f->data();
    }
    m_conn->sendMessage(Message::S_BOARD_CONTENT, m);
}

void NetworkPlayer::sendBoardDescription(GameBoard *board, QVariantMap descriptions)
{
    QVariantMap boardMap;
    for (auto f : board->allFields()) {
        boardMap[QString::number(f->id())] << *f->data();
    }
    QVariantMap map;
    map["board"] << boardMap;
    map["descriptions"] << descriptions;
    m_conn->sendMessage(Message::S_BOARD_DESCRIPTION, map);
}

void NetworkPlayer::sendCharacter(Character *c)
{
    QVariant v;
    v << *(c->data());
    m_conn->sendMessage(Message::S_CHARACTER_DATA, v);
}

void NetworkPlayer::sendSetting(GameSettingData data)
{
    QVariant v;
    v << data;
    m_conn->sendMessage(Message::S_SETTING_DATA, v);
}

void NetworkPlayer::nextRound()
{
    sendText("Next Round");
}

void NetworkPlayer::gamePhaseChanged(AH::GamePhase phase)
{
    QVariant v;
    v << phase;
    m_conn->sendMessage(Message::S_PHASE_CHANGE, v);
}

void NetworkPlayer::firstPlayerChanged(const Player *player)
{
    if (player == this) {
        sendText("You are the first player");
    } else {
        sendText("First Player is " + player->id());
    }
}

void NetworkPlayer::currentPlayerChanged(const Player *player)
{
    QVariant v;
    v << player->id();
    m_conn->sendMessage(Message::S_PLAYER_CHANGE, v);
}

void NetworkPlayer::actionStart(const GameAction *action, QString desc)
{
    sendText(m_formatter->formatActionStart(action, desc));
}

void NetworkPlayer::actionUpdate(const GameAction *action, QString desc)
{
    sendText(m_formatter->formatActionUpdate(action, desc));
}

void NetworkPlayer::actionFinish(const GameAction *action, QString desc)
{
    sendText(m_formatter->formatActionFinish(action, desc));
}

void NetworkPlayer::actionExecute(const GameAction *action, QString desc)
{
    sendText(m_formatter->formatActionExecute(action, desc));
}

void NetworkPlayer::notifySimple(const QString &str, Player *p, const QString &desc)
{
    sendText(m_formatter->formatSimple(str, p, desc));
}

void NetworkPlayer::objectsInvalidated(QStringList ids)
{
    m_conn->sendMessage(Message::S_INVALIDATE_OBJECTS, QVariant(ids));
}

void NetworkPlayer::objectTypeInvalidated(RequestObjectsData::ObjectType type)
{
    m_conn->sendMessage(Message::S_INVALIDATE_OBJECTS_BY_TYPE, QVariant(type));
}

void NetworkPlayer::setTempData(const QString &data)
{
    QVariant m;
    m << data;
    m_conn->sendMessage(Message::S_SET_TEMP_DATA, m);
}

void NetworkPlayer::clearTempData()
{
    m_conn->sendMessage(Message::S_CLEAR_TEMP_DATA, QVariant());
}

bool NetworkPlayer::acknowledgeMythos(const MythosCard *m, QObject *observer)
{
    QVariant v;
    v << *(m->data());
    m_conn->sendMessage(AH::Common::Message::S_ACKNOWLEDGE_MYTHOS, v);
    m_ackReceiver = observer;
    return false;
}

bool NetworkPlayer::acknowledgeMonsterMovement(Monster *m, QObject *observer)
{
    QVariant v;
    v << *(m->data());
    m_conn->sendMessage(AH::Common::Message::S_ACKNOWLEDGE_MONSTER_MOVEMENT, v);
    m_ackReceiver = observer;
    return false;
}

void NetworkPlayer::abortAcknowledge()
{
    m_conn->sendMessage(AH::Common::Message::S_ABORT_ACKNOWLEDGE, QVariant());
    m_ackReceiver = nullptr;
}

void NetworkPlayer::notifyWon(QString msg)
{
    m_conn->sendMessage(AH::Common::Message::S_WON, msg);
}

void NetworkPlayer::notifyLost(QString msg)
{
    m_conn->sendMessage(AH::Common::Message::S_LOST, msg);
}

void NetworkPlayer::notifyDied(Player *p)
{
    if (p == this) {
        m_conn->sendMessage(AH::Common::Message::S_DIED, "You are dead");
    } else {
        m_conn->sendMessage(AH::Common::Message::S_DIED, m_formatter->formatSimple("{C} is dead", p));
    }
}

void NetworkPlayer::flush()
{
    m_conn->flush();
}

void NetworkPlayer::notifyAlert(const QString &msg, Player *p, const QString &desc)
{
    m_conn->sendMessage(AH::Common::Message::S_GAME_ALERT, m_formatter->formatSimple(msg, p, desc));
}

void NetworkPlayer::notifySpecific(const QString &strThat, const QString &strOther, Player *that, const QString &desc)
{
    if (this == that) {
        notifyAlert(strThat, that, desc);
    } else {
        notifySimple(strOther, that, desc);
    }
}

DieTestUpdateData NetworkPlayer::dieRollStart(const DieRollTestData &test)
{
    // same as update...
    return dieRollUpdate(test);
}

DieTestUpdateData NetworkPlayer::dieRollUpdate(const DieRollTestData &test)
{
    // Start with display
    dieRollFinish(test);

    // Ask for updates
    AH::Common::Message resp;
    QList<Message::Type> l;
    l << Message::C_DIE_ROLL_UPDATE;
    bool ok = awaitResponse(resp, l);
    if (ok) {
        // Find investigator by its id
        DieTestUpdateData upd;
        resp.payload >> upd;
        return upd;
    }
    return DieTestUpdateData();
}

void NetworkPlayer::dieRollFinish(const DieRollTestData &test)
{
    QVariant v;
    v << test;
    m_conn->sendMessage(Message::S_DIE_ROLL_INFO, v);
}

Investigator *NetworkPlayer::chooseInvestigator(QList<Investigator *> invs)
{
    QVariant v;
    v << invs;
    m_conn->sendMessage(Message::S_CHOOSE_INVESTIGATOR, v);

    AH::Common::Message resp;
    QList<Message::Type> l;
    l << Message::C_SELECT_INVESTIGATOR;
    bool ok = awaitResponse(resp, l);
    if (ok) {
        // Find investigator by its id
        QString id = resp.payload.toString();
        for (auto i : invs) {
            if (i->id() == id) {
                return i;
            }
        }
    }

    return nullptr;
}

bool NetworkPlayer::chooseWeapons(QList<GameObject *> weapons, ModifiedPropertyValue hands, QStringList &selected)
{
    QVariantMap m;
    m["hands"] << hands.toModifiedPropertyValueData();
    m["weapons"] << weapons;
    QVariant v;
    v << m;
    m_conn->sendMessage(Message::S_CHOOSE_WEAPONS, v);

    AH::Common::Message resp;
    QList<Message::Type> l;
    l << Message::C_SELECT_WEAPONS << Message::C_CANCEL_WEAPONS;
    bool ok = awaitResponse(resp, l);
    if (ok) {
        if (resp.type == Message::C_CANCEL_WEAPONS) {
            return false;
        }

        selected = resp.payload.toStringList();
        return true;
    }
    return false;
}

GameOption *NetworkPlayer::chooseOption(QList<GameOption *> options)
{
    QVariant v;
    v << options;
    m_conn->sendMessage(Message::S_CHOOSE_OPTION, v);

    AH::Common::Message resp;
    QList<Message::Type> l;
    l << Message::C_SELECT_OPTION;
    bool ok = awaitResponse(resp, l);
    if (ok) {
        // Find investigator by its id
        QString id = resp.payload.toString();
        for (auto i : options) {
            if (i->id() == id) {
                return i;
            }
        }
    }
    return nullptr;
}

QList<int> NetworkPlayer::chooseFocus(QList<AttributeSlider> sliders, int totalFocus)
{
    QVariantMap map;

    // Must convert AttributeSlider list to AttributeSliderData list
    QList<AttributeSliderData> dataList;
    foreach (const AttributeSlider &s, sliders) {
        dataList << *s.data();
    }

    map["focusAmount"] << totalFocus;
    map["focusSliders"] << dataList;
    m_conn->sendMessage(Message::S_CHOOSE_FOCUS, map);

    AH::Common::Message resp;
    QList<Message::Type> l;
    l << Message::C_SELECT_FOCUS;
    bool ok = awaitResponse(resp, l);
    QList<int> diffs;
    if (ok) {
        resp.payload >> diffs;
    }
    return diffs;
}

MovementPath NetworkPlayer::chooseMovement(GameField *start, int movement)
{
    QVariantMap map;
    map["startId"] << start->id();
    map["movementPoints"] << movement;
    m_conn->sendMessage(Message::S_CHOOSE_MOVEMENT_PATH, map);

    AH::Common::Message resp;
    QList<Message::Type> l;
    l << Message::C_MOVE_PATH;
    bool ok = awaitResponse(resp, l);
    MovementPath ret;
    if (ok) {
        QList<FieldData::FieldID> lst;
        resp.payload >> lst;
        for (auto id : lst) {
            GameField *f = gGame->board()->field(id);
            ret << f;
        }
    }
    return ret;
}

QString NetworkPlayer::chooseEncounterOption(EncounterData *enc)
{
    QVariant v;
    v << *enc->data();
    m_conn->sendMessage(Message::S_CHOOSE_ENCOUNTER, v);

    AH::Common::Message resp;
    QList<Message::Type> l;
    l << Message::C_SELECT_ENCOUNTER;
    bool ok = awaitResponse(resp, l);
    if (ok) {
        // Find investigator by its id
        QString id = resp.payload.toString();
        return id;
    }
    return QString::null;
}

QString NetworkPlayer::chooseMonster(QString desc, QList<MonsterData> monsters)
{
    QVariantMap v;
    v["description"] << desc;
    v["monsters"] << monsters;
    m_conn->sendMessage(Message::S_CHOOSE_MONSTER, v);

    AH::Common::Message resp;
    QList<Message::Type> l;
    l << Message::C_SELECT_MONSTER;
    bool ok = awaitResponse(resp, l);
    if (ok) {
        QString id = resp.payload.toString();
        return id;
    }
    return QString::null;
}

/*
CostList NetworkPlayer::choosePayment(const Cost &c)
{
    QVariant m;
    m << c;
    m_conn->sendMessage(Message::S_CHOOSE_PAYMENT, m);

    AH::Common::Message resp;
    QList<Message::Type> l;
    l << Message::C_SELECT_PAYMENT;
    bool ok = awaitResponse(resp, l);
    if (ok) {
        qint32 i;
        resp.payload >> i;
        if (i >= 0 && i < c.getAlternatives().size()) {
            return c.getAlternatives()[i];
        }
    }
    return CostList();
}
*/

AH::Common::PropertyValueData::Property NetworkPlayer::chooseSkill(QList<ModifiedPropertyValueData> options)
{
    QVariant m;
    m << options;
    m_conn->sendMessage(Message::S_CHOOSE_SKILL, m);

    AH::Common::Message resp;
    QList<Message::Type> l;
    l << Message::C_SELECT_SKILL;
    bool ok = awaitResponse(resp, l);
    if (ok) {
        AH::Common::PropertyValueData::Property s;
        resp.payload >> s;
        return s;
    }
    return AH::Common::PropertyValueData::NoProperty;
}

ChoiceResponseData NetworkPlayer::offerChoice(ChoiceData choice)
{
    QVariant m;
    m << choice;
    m_conn->sendMessage(Message::S_OFFER_CHOICE, m);

    AH::Common::Message resp;
    QList<Message::Type> l;
    l << Message::C_CANCEL_CHOICE << Message::C_SELECT_CHOICE;
    bool ok = awaitResponse(resp, l);
    if (ok) {
        ChoiceResponseData ret;
        if (resp.type == Message::C_SELECT_CHOICE) {
            //resp.payload >> ret;
            ret = resp.payload;
        }
        return ret;
    }
    return ChoiceResponseData();
}

TradeData NetworkPlayer::offerTrade(TradeData trade)
{
    QVariant m;
    m << trade;
    m_conn->sendMessage(AH::Common::Message::S_TRADE, m);

    AH::Common::Message resp;
    QList<Message::Type> l;
    l << Message::C_CANCEL_TRADE << Message::C_TRADE;
    bool ok = awaitResponse(resp, l);
    TradeData ret;
    if (ok) {
        if (resp.type == Message::C_TRADE) {
            resp.payload >> ret;
        }
    }
    return ret;
}

void NetworkPlayer::tradeCanceled(const QString &name)
{
    m_conn->sendMessage(AH::Common::Message::S_CANCEL_TRADE, name);
    //Message r;
    //awaitResponse(r, QList<Message::Type>() << Message::C_ACKNOWLEDGED);
}

void NetworkPlayer::handleMessage(const Message &msg)
{
    Qt::ConnectionType t = (QThread::currentThread() == this->thread()) ? Qt::DirectConnection : Qt::QueuedConnection;
    QMetaObject::invokeMethod(this, "doHandleMessage", t, Q_ARG(AH::Common::Message, msg));
}

void NetworkPlayer::destroy()
{
    m_game->removePlayer(this);
    m_bWaitSuccessful = false;
    emit receivedWaitedMesage();
}

void NetworkPlayer::doHandleMessage(const Message &msg)
{
    if (msg.type == Message::C_CONFIRM_ACTIVE) {
        m_killTimer->stop();
        m_promptTimer->start();
    } else if (msg.type == Message::C_ACKNOWLEDGED) {
        if (m_ackReceiver) {
            qApp->postEvent(m_ackReceiver, new AcknowledgeEvent(this));
        }
        abortAcknowledge();
    } else if (msg.type == Message::C_SET_AUTOSKIP) {
        PlayerData::AutoSkipData d;
        msg.payload >> d;
        this->m_autoSkip = d;
    } else if (m_waitMsgTypes.contains(msg.type)) {
        m_waitMsg = msg;
        m_bWaitSuccessful = true;
        emit receivedWaitedMesage();
    } else {
        qWarning() << "Network player received unexpected message: " << Message::msg_to_str(msg.type);
    }

    // TODO: Handle other messages
}

void NetworkPlayer::sendText(const QString &txt)
{
    if (!txt.isEmpty())
        m_conn->sendMessage(AH::Common::Message::S_TEXT_MESSAGE, txt);
}

bool NetworkPlayer::awaitResponse(Message &outMsg, const QList<Message::Type> &acceptTypes)
{
    //Is this true?: Q_ASSERT_X(QThread::currentThread() != this->thread(), "NetworkPlayer::awaitResponse", "Waiting in same thread will cause a DEADLOCK");

    m_conn->flush();

    m_waitMsgTypes = acceptTypes;
    QEventLoop w;
    connect(this, SIGNAL(receivedWaitedMesage()), &w, SLOT(quit()));

    m_promptTimer->start();

    m_bWaitSuccessful = true;
    w.exec();

    m_promptTimer->stop();
    m_killTimer->stop();

    if (m_bWaitSuccessful) {
        outMsg = m_waitMsg;
        return true;
    }
    return false;
}

void NetworkPlayer::promptActive()
{
    m_conn->sendMessage(Message::S_PROMPT_ACTIVE, QVariant());
    m_killTimer->start(KILL_TIMEOUT);
}

void NetworkPlayer::killPlayer()
{
    m_conn->sendMessage(Message::S_KILL, QVariant());
    m_bWaitSuccessful = false;
    emit receivedWaitedMesage();
    m_conn->close();
    m_game->removePlayer(this);
}

