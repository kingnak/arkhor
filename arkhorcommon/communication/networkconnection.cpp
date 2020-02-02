#include "networkconnection.h"
#include <QTimerEvent>
#include <QtEndian>
#include <QThread>
#include <QDataStream>

namespace AH { namespace Common {

QString msg_to_str(AH::Common::Message::Type t) {
    switch (t) {
    case AH::Common::Message::Type::NO_MESSAGE: return "NO_MESSAGE";
    case AH::Common::Message::Type::C_VERSION: return "C_VERSION";
    case AH::Common::Message::Type::S_VERSION: return "S_VERSION";
    case AH::Common::Message::Type::S_TEXT_MESSAGE: return "S_TEXT_MESSAGE";
    case AH::Common::Message::Type::MSG_PING: return "MSG_PING";
    case AH::Common::Message::Type::MSG_PONG: return "MSG_PONG";
    case AH::Common::Message::Type::S_PROMPT_ACTIVE: return "S_PROMPT_ACTIVE";
    case AH::Common::Message::Type::C_CONFIRM_ACTIVE: return "C_CONFIRM_ACTIVE";
    case AH::Common::Message::Type::S_KILL: return "S_KILL";
    case AH::Common::Message::Type::S_PLAYER_REMOVED: return "S_PLAYER_REMOVED";
    case AH::Common::Message::Type::C_REGISTER_PLAYER: return "C_REGISTER_PLAYER";
    case AH::Common::Message::Type::S_REGISTER_PLAYER_SUCCESSFUL: return "S_REGISTER_PLAYER_SUCCESSFUL";
    case AH::Common::Message::Type::S_REGISTER_PLAYER_FAILED: return "S_REGISTER_PLAYER_FAILED";
    case AH::Common::Message::Type::C_START_GAME: return "C_START_GAME";
    case AH::Common::Message::Type::S_GAME_STARTED: return "S_GAME_STARTED";
    case AH::Common::Message::Type::S_BOARD_CONTENT: return "S_BOARD_CONTENT";
    case AH::Common::Message::Type::S_BOARD_DESCRIPTION: return "S_BOARD_DESCRIPTION";
    case AH::Common::Message::Type::S_CHARACTER_DATA: return "S_CHARACTER_DATA";
    case AH::Common::Message::Type::S_SETTING_DATA: return "S_SETTING_DATA";
    case AH::Common::Message::Type::C_REQUEST_OBJECTS: return "C_REQUEST_OBJECTS";
    case AH::Common::Message::Type::S_DESCRIBE_OBJECTS: return "S_DESCRIBE_OBJECTS";
    case AH::Common::Message::Type::S_INVALIDATE_OBJECTS: return "S_INVALIDATE_OBJECTS";
    case AH::Common::Message::Type::S_INVALIDATE_OBJECTS_BY_TYPE: return "S_INVALIDATE_OBJECTS_BY_TYPE";
    case AH::Common::Message::Type::S_CHOOSE_INVESTIGATOR: return "S_CHOOSE_INVESTIGATOR";
    case AH::Common::Message::Type::C_SELECT_INVESTIGATOR: return "C_SELECT_INVESTIGATOR";
    case AH::Common::Message::Type::S_PLAYER_CHAR_INSTANTIATED: return "S_PLAYER_CHAR_INSTANTIATED";
    case AH::Common::Message::Type::S_DIE_ROLL_INFO: return "S_DIE_ROLL_INFO";
    case AH::Common::Message::Type::C_DIE_ROLL_UPDATE: return "C_DIE_ROLL_UPDATE";
    case AH::Common::Message::Type::S_GAME_START: return "S_GAME_START";
    case AH::Common::Message::Type::S_PHASE_CHANGE: return "S_PHASE_CHANGE";
    case AH::Common::Message::Type::S_PLAYER_CHANGE: return "S_PLAYER_CHANGE";
    case AH::Common::Message::Type::S_CHOOSE_OPTION: return "S_CHOOSE_OPTION";
    case AH::Common::Message::Type::C_SELECT_OPTION: return "C_SELECT_OPTION";
    case AH::Common::Message::Type::S_CHOOSE_SKILL: return "S_CHOOSE_SKILL";
    case AH::Common::Message::Type::C_SELECT_SKILL: return "C_SELECT_SKILL";
    //case AH::Common::Message::Type::S_CHOOSE_PAYMENT: return "S_CHOOSE_PAYMENT";
    //case AH::Common::Message::Type::C_SELECT_PAYMENT: return "C_SELECT_PAYMENT";
    case AH::Common::Message::Type::S_CHOOSE_MOVEMENT_PATH: return "S_CHOOSE_MOVEMENT_PATH";
    case AH::Common::Message::Type::C_MOVE_PATH: return "C_MOVE_PATH";
    case AH::Common::Message::Type::S_CHOOSE_FOCUS: return "S_CHOOSE_FOCUS";
    case AH::Common::Message::Type::C_SELECT_FOCUS: return "C_SELECT_FOCUS";
    case AH::Common::Message::Type::S_CHOOSE_WEAPONS: return "S_CHOOSE_WEAPONS";
    case AH::Common::Message::Type::C_SELECT_WEAPONS: return "C_SELECT_WEAPONS";
    case AH::Common::Message::Type::C_CANCEL_WEAPONS: return "C_CANCEL_WEAPONS";
    case AH::Common::Message::Type::S_CHOOSE_ENCOUNTER: return "S_CHOOSE_ENCOUNTER";
    case AH::Common::Message::Type::C_SELECT_ENCOUNTER: return "C_SELECT_ENCOUNTER";
    case AH::Common::Message::Type::S_CHOOSE_MONSTER: return "S_CHOOSE_MONSTER";
    case AH::Common::Message::Type::C_SELECT_MONSTER: return "C_SELECT_MONSTER";
    case AH::Common::Message::Type::S_ACKNOWLEDGE_MYTHOS: return "S_ACKNOWLEDGE_MYTHOS";
    case AH::Common::Message::Type::S_ACKNOWLEDGE_MONSTER_MOVEMENT: return "S_ACKNOWLEDGE_MONSTER_MOVEMENT";
    case AH::Common::Message::Type::S_ABORT_ACKNOWLEDGE: return "S_ABORT_ACKNOWLEDGE";
    case AH::Common::Message::Type::C_ACKNOWLEDGED: return "C_ACKNOWLEDGED";
    case AH::Common::Message::Type::S_OFFER_CHOICE: return "S_OFFER_CHOICE";
    case AH::Common::Message::Type::C_SELECT_CHOICE: return "C_SELECT_CHOICE";
    case AH::Common::Message::Type::C_CANCEL_CHOICE: return "C_CANCEL_CHOICE";
    case AH::Common::Message::Type::S_SET_TEMP_DATA: return "S_SET_TEMP_DATA";
    case AH::Common::Message::Type::S_CLEAR_TEMP_DATA: return "S_CLEAR_TEMP_DATA";
    case AH::Common::Message::Type::S_WON: return "S_WON";
    case AH::Common::Message::Type::S_LOST: return "S_LOST";
    case AH::Common::Message::Type::S_DIED: return "S_DIED";
    case AH::Common::Message::Type::S_GAME_ALERT: return "S_GAME_ALERT";
    case AH::Common::Message::Type::C_SET_AUTOSKIP: return "C_SET_AUTOSKIP";
    case AH::Common::Message::Type::S_TRADE: return "S_TRADE";
    case AH::Common::Message::Type::C_TRADE: return "C_TRADE";
    case AH::Common::Message::Type::S_CANCEL_TRADE: return "S_CANCEL_TRADE";
    case AH::Common::Message::Type::C_CANCEL_TRADE: return "C_CANCEL_TRADE";
    case AH::Common::Message::Type::DONT_UNDERSTAND: return "DONT_UNDERSTAND";
    case AH::Common::Message::Type::INVALID_MESSAGE: return "INVALID_MESSAGE";
    default: return QString("?? %d").arg(t);
    }
}

NetworkConnection::NetworkConnection(qintptr socketDescriptor, QObject *parent)
:   QObject(parent),
    //QTcpSocket(parent),
    m_sockDesc(socketDescriptor),
    m_socket(NULL),
    m_state(ReadHeader),
    m_lengthNeeded(0),
    m_pingTimer(NULL),
    m_transferTimer(0),
    m_doPing(false)
{
}

NetworkConnection::NetworkConnection(QTcpSocket *sock, QObject *parent)
:   QObject(parent),
    //QTcpSocket(parent),
    m_sockDesc(-1),
    m_socket(sock),
    m_state(ReadHeader),
    m_lengthNeeded(0),
    m_pingTimer(NULL),
    m_transferTimer(0),
    m_doPing(false)
{
    if (m_socket) {
        m_socket->setParent(this);
    }
}

void NetworkConnection::receivedMessage(const Message &msg)
{
    emit messageReceived(msg);
}

void NetworkConnection::startup()
{
    if (m_sockDesc > 0) {
        m_socket = new QTcpSocket(this);
        m_socket->setSocketDescriptor(m_sockDesc);
    }

    m_state = ReadHeader;
    m_lengthNeeded = 0;

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(dataReady()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(stop()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(stop()));

    if (m_socket->state() == QTcpSocket::ConnectedState) {
        startPinging();
    } else {
        connect(m_socket, SIGNAL(connected()), this, SLOT(startPinging()));
    }
}

void NetworkConnection::cleanup()
{
    //delete m_socket;
}

void NetworkConnection::close()
{
    QMetaObject::invokeMethod(this, "doClose");
    emit shutdown();
}

void NetworkConnection::abort()
{
    QMetaObject::invokeMethod(this, "doAbort");
    emit shutdown();
}

void NetworkConnection::stop()
{
    if (!m_socket->isValid()) {
        qDebug() << "Socket Error:" << m_socket->errorString();
    }
    this->abort();
    emit shutdown();
}

void NetworkConnection::startPinging()
{
    if (!m_doPing) return;

    if (m_socket && m_socket->state() == QTcpSocket::ConnectedState) {
        if (!m_pingTimer) {
            m_pingTimer = new QTimer(this);
        }
        if (!m_pingTimer->isActive()) {
            connect(m_pingTimer, SIGNAL(timeout()), this, SLOT(sendPing()));
            m_pongTime.start();
            m_pingTimer->start(PING_TIMEOUT);
        }
    }
}

void NetworkConnection::sendPing()
{
    if (m_pongTime.elapsed() > PONG_TIMEOUT) {
        // Connect aborted?
        abort();
        return;
    }

    sendMessage(Message::MSG_PING, QVariant());
}

void NetworkConnection::sendMessage(const Message &msg)
{
   QMetaObject::invokeMethod(this, "doSendMessage", Q_ARG(const AH::Common::Message &, msg));
}

void NetworkConnection::doSendMessage(const Message &msg)
{
#ifndef NDEBUG
    if (msg.type != AH::Common::Message::Type::MSG_PING && msg.type != AH::Common::Message::MSG_PONG)
        qDebug(qPrintable(QString("%1: Send %2").arg((qulonglong)m_socket, 16, 16, QChar('0')).arg(msg_to_str(msg.type))));
    //qDebug() << "Sending Message " << msg_to_str(msg.type);
#endif

    Q_ASSERT_X(QThread::currentThread() == this->thread(), "NetworkConnection::doSendMessage", "Can only send from own thread!");

    QVariant v;
    v << msg;

    QByteArray buf;
    QDataStream ds(&buf, QIODevice::WriteOnly);
    ds.setVersion(QDataStream::Qt_4_6);
    ds << v;

    quint32 mag = MAGICK;
    quint32 len = buf.size();

    //qDebug() << "sending " << len << " bytes message";

    mag = qToLittleEndian(mag);
    len = qToLittleEndian(len);

    char magChars[sizeof(mag)];
    char lenChars[sizeof(len)];
    intToBytes(mag, magChars);
    intToBytes(len, lenChars);

    m_socket->write(magChars, sizeof(magChars) / sizeof(*magChars));
    m_socket->write(lenChars, sizeof(lenChars) / sizeof(*lenChars));
    //m_socket->write(reinterpret_cast<const char *>(&len), sizeof(len));
    m_socket->write(buf);
}

void NetworkConnection::doClose()
{
    m_socket->flush();
    m_socket->close();
}

void NetworkConnection::doAbort()
{
    m_socket->abort();
}

void NetworkConnection::sendMessage(Message::Type type, const QVariant &payload)
{
    Message m;
    m.type = type;
    m.payload = payload;
    sendMessage(m);
}

void NetworkConnection::timerEvent(QTimerEvent *timerEvent)
{
    if (timerEvent->timerId() == m_transferTimer) {
        // Transfer timeout!
        killTimer(m_transferTimer);
        m_transferTimer = 0;
        abort();
    }
}

void NetworkConnection::startTransferTimeout()
{
    stopTransferTimeout();
    m_transferTimer = startTimer(TRANSFER_TIMEOUT);
}

void NetworkConnection::stopTransferTimeout()
{
    if (m_transferTimer) {
        killTimer(m_transferTimer);
        m_transferTimer = 0;
    }
}

void NetworkConnection::dataReady()
{
    Q_ASSERT_X(QThread::currentThread() == this->thread(), "NetworkConnection::dataReady", "Can only receive in own thread!");

    do {
        if (m_state == ReadHeader)
            if (!readProtocolHeader())
                return;
        if (m_state == ReadLength)
            if (!readLength())
                return;

        if (m_state == ReadMessage) {
            if (!hasEnoughData())
                return;

            processMessage();
        }
    } while (m_socket->bytesAvailable() > 0);
}

qint64 NetworkConnection::readDataIntoBuffer(qint64 maxSize)
{
     int numBytesBeforeRead = m_buffer.size();
     if (numBytesBeforeRead == maxSize) {
         return 0;
     }

     while (m_socket->bytesAvailable() > 0 && m_buffer.size() < maxSize) {
         m_buffer.append(m_socket->read(maxSize - m_buffer.size()));
     }
     return m_buffer.size() - numBytesBeforeRead;
}

bool NetworkConnection::readProtocolHeader()
{
    // No timeout at start of message
    stopTransferTimeout();

    if (readDataIntoBuffer(MAGICK_SIZE) <= 0) {
        // Need rest of data in short time!
        startTransferTimeout();
        return false;
    }

    //quint32 readMagick = qFromLittleEndian(*reinterpret_cast<const quint32 *> (m_buffer.constData()));
    quint32 readMagick = byteToInt<quint32>(m_buffer.constData(), MAGICK_SIZE);
    readMagick = qFromLittleEndian(readMagick);

    if (readMagick != MAGICK) {
        abort();
        return false;
    }

    m_buffer.clear();

    m_state = ReadLength;
    return true;
}

bool NetworkConnection::readLength()
{
    stopTransferTimeout();
    if (readDataIntoBuffer(LENGTH_SIZE) <= 0) {
        startTransferTimeout();
        return false;
    }

    //quint32 readLen = qFromLittleEndian(*reinterpret_cast<const quint32 *> (m_buffer.constData()));
    quint32 readLen = byteToInt<quint32>(m_buffer.constData(), LENGTH_SIZE);
    readLen = qFromLittleEndian(readLen);

    m_buffer.clear();

    m_state = ReadMessage;
    m_lengthNeeded = readLen;
    return true;
}

bool NetworkConnection::hasEnoughData()
{
    stopTransferTimeout();
    if (m_socket->bytesAvailable() < m_lengthNeeded) {
        startTransferTimeout();
        return false;
    }

    return true;
}

void NetworkConnection::processMessage()
{
    m_buffer = m_socket->read(m_lengthNeeded);
    if (static_cast<quint32> (m_buffer.size()) != m_lengthNeeded) {
        stopTransferTimeout();
        abort();
        return;
    }

    QDataStream ds(&m_buffer, QIODevice::ReadOnly);
    ds.setVersion(QDataStream::Qt_4_6);
    QVariant v;
    ds >> v;

    m_buffer.clear();
    m_state = ReadHeader;

    Message msg;
    v >> msg;

#ifndef NDEBUG
    //qDebug() << "Received Message " << msg.type;
    if (msg.type != AH::Common::Message::Type::MSG_PING && msg.type != AH::Common::Message::MSG_PONG)
        qDebug(qPrintable(QString("%1: Recv %2").arg((qulonglong)m_socket, 16, 16, QChar('0')).arg(msg_to_str(msg.type))));
#endif

    handleMessage(msg);
}

void NetworkConnection::handleMessage(const Message &msg)
{
    if (msg.type == Message::MSG_PING) {
        sendMessage(Message::MSG_PONG, QVariant());
    } else if (msg.type == Message::MSG_PONG) {
        m_pongTime.restart();
    } else {
        receivedMessage(msg);
    }
}

template <typename T, size_t N>
void NetworkConnection::intToBytes(T val, char(&dest)[N])
{
    for (size_t i = 0; i < N; ++i) {
        dest[N-i-1] = val & 0xFF;
        val >>= 8;
    }
}

template <typename T, size_t N>
T NetworkConnection::byteToInt(const char (&src)[N])
{
    return byteToInt(src, N);
}

template <typename T>
T NetworkConnection::byteToInt(const char *src, size_t N)
{
    T ret = 0;
    for (size_t i = 0; i < N; ++i) {
        ret <<= 8;
        ret |= (src[i]) & 0xFF;
    }
    return ret;
}

}}
