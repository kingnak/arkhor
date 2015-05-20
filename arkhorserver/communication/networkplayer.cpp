#include "networkplayer.h"
#include "clientconnection.h"
#include "game/game.h"
#include "game/gameaction.h"
#include <cost.h>
#include "game/investigator.h"
#include "character.h"
#include "game/gameoption.h"
#include <QEventLoop>
#include <QThread>
#include <QTimerEvent>

using namespace AH::Common;

NetworkPlayer::NetworkPlayer()
:   m_game(NULL),
    m_conn(NULL)
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
}

void NetworkPlayer::sendBoard(GameBoard *board)
{
    QVariantMap m;
    foreach (GameField *f, board->allFields()) {
        m[QString::number(f->id())] << *f->data();
    }
    m_conn->sendMessage(Message::S_BOARD_CONTENT, m);
}

void NetworkPlayer::sendCharacter(Character *c)
{
    QVariant v;
    v << *(c->data());
    m_conn->sendMessage(Message::S_CHARACTER_DATA, v);
}

void NetworkPlayer::nextRound()
{
    sendText("Next Round");
}

void NetworkPlayer::gamePhaseChaned(AH::GamePhase phase)
{
    QString s;
    switch (phase) {
    case AH::Upkeep: s = "Upkeep"; break;
    case AH::Movement: s = "Movement"; break;
    case AH::ArkhamEncountery: s = "Arkham Encountery"; break;
    case AH::OtherWorldEncountery: s = "Other World Encountery"; break;
    case AH::Mythos: s = "Mythos"; break;
    default: s = QString("Unknown (%1)").arg(phase); break;
    }

    sendText("New Phase: " + s);
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
    if (player == this) {
        sendText("You are the current player");
    } else {
        sendText("Current Player is " + player->id());
    }
}

void NetworkPlayer::actionStart(const GameAction *action, QString desc)
{
    sendText(QString("Starting action %1: %2").arg(action->name(), desc));
}

void NetworkPlayer::actionUpdate(const GameAction *action, QString desc)
{
    sendText(QString("Updating action %1: %2").arg(action->name(), desc));
}

void NetworkPlayer::actionFinish(const GameAction *action, QString desc)
{
    sendText(QString("Finishing action %1: %2").arg(action->name(), desc));
}

void NetworkPlayer::actionExecute(const GameAction *action, QString desc)
{
    sendText(QString("Executing action %1: %2").arg(action->name(), desc));
}

DieTestUpdateData NetworkPlayer::dieRollStart(const DieRollTestData test)
{
    // same as update...
    return dieRollUpdate(test);
}

DieTestUpdateData NetworkPlayer::dieRollUpdate(const DieRollTestData test)
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

void NetworkPlayer::dieRollFinish(const DieRollTestData test)
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
        foreach (Investigator *i, invs) {
            if (i->id() == id) {
                return i;
            }
        }
    }

    return NULL;
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
        foreach (GameOption *i, options) {
            if (i->id() == id) {
                return i;
            }
        }
    }
    return NULL;
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
        foreach (FieldData::FieldID id, lst) {
            GameField *f = gGame->board()->field(id);
            ret << f;
        }
    }
    return ret;
}

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

void NetworkPlayer::handleMessage(Message msg)
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

void NetworkPlayer::doHandleMessage(Message msg)
{
    if (msg.type == Message::C_CONFIRM_ACTIVE) {
        m_killTimer->stop();
        m_promptTimer->start();
    } else if (m_waitMsgTypes.contains(msg.type)) {
        m_waitMsg = msg;
        m_bWaitSuccessful = true;
        emit receivedWaitedMesage();
        return;
    }

    // TODO: Handle other messages
}

void NetworkPlayer::sendText(QString txt)
{
    m_conn->sendMessage(AH::Common::Message::S_TEXT_MESSAGE, txt);
}

bool NetworkPlayer::awaitResponse(Message &outMsg, QList<Message::Type> acceptTypes)
{
    //Is this true?: Q_ASSERT_X(QThread::currentThread() != this->thread(), "NetworkPlayer::awaitResponse", "Waiting in same thread will cause a DEADLOCK");

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

