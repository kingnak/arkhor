#include "httpserver.h"
#include <QDebug>
#include <QThreadPool>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTimerEvent>

#include <communication/httpparser.h>
#include <utils/cleanupthread.h>

using namespace AH::Common;

Q_DECLARE_METATYPE(HttpClientConnection::VoidFunc);
Q_DECLARE_METATYPE(QTcpSocket *);

HttpServer::HttpServer()
{
    qRegisterMetaType<HttpClientConnection::VoidFunc>();
    qRegisterMetaType<QTcpSocket*>();
}

void HttpServer::start()
{
    bool ok = listen(QHostAddress::Any, ServerPort);
    if (!ok) {
        qCritical() << QString("Cannot listen on HTTP server port %1").arg(ServerPort);
    }
}

void HttpServer::stop()
{
    this->close();
}

void HttpServer::incomingConnection(qintptr socketDescriptor)
{
    // Handle in pooled thread
    QThreadPool::globalInstance()->start([=]() {
        QTcpSocket *s = new QTcpSocket;
        s->setSocketDescriptor(socketDescriptor);
        handleConnection(s);
    });
}

void HttpServer::handleConnection(QTcpSocket *socket)
{
    auto reply = [socket](HttpResponse res){
        res.writeTo(socket);
        socket->flush();
        socket->close();
        //qDebug() << "Thread " << QThread::currentThreadId() << " socket " << socket << ": handleConnection DELETE";
        delete socket;
    };

    //qDebug() << "Thread " << QThread::currentThreadId() << " socket " << socket << ": handleConnection";

    auto req = HttpRequest::readMessage(socket);
    if (!req) {
        qCritical()
                << "Error in HTTP request:"
                << req.error()
                << ", request:"
                << (req.method()==HttpRequest::Method::GET ? "GET" : "POST")
                << req.resource()
                << "("
                << req.content().length()
                << " bytes content)"
                   ;
        auto res = req.createResponseFromError();
        reply(res);
        return;
    }

    qDebug() << "Received HTTP " << (req.method()==HttpRequest::Method::GET ? "GET" : "POST") << "request. Resource: "
             << req.resource()
             << ", content: "
             << req.content().length() << " bytes";


    QStringList parts = req.resource().split('/', QString::SplitBehavior::SkipEmptyParts);
    QString ep = parts.value(0);
    QString cId = parts.value(1);

    HttpClientConnection *client = nullptr;
    if (cId.length() > 0) {
        client = getClient(cId);
        if (!client) {
            qCritical()
                    << "Invalid client id: "
                    << cId
                    << " for request at "
                    << ep;

            HttpResponse r(HttpResponse::Code::BadRequest);
            r.addCorsHeaders();
            r.setContent("Invalid client id");
            reply(r);
            return;
        }
    }


    Message msg;
    msg.type = Message::Type::NO_MESSAGE;
    if (req.content().length() > 0) {
        QJsonParseError err;
        QJsonDocument json = QJsonDocument::fromJson(req.content(), &err);
        if (err.error != QJsonParseError::ParseError::NoError) {
            qCritical()
                    << "Error in HTTP request, invalid JSON: "
                    << err.errorString()
                    << " for request at "
                    << ep;

            HttpResponse r(HttpResponse::Code::BadRequest);
            r.addCorsHeaders();
            r.setContent("Invalid JSON message");
            reply(r);
            return;
        }
        QVariant v = json.toVariant();
        v >> msg;
    }

    if (ep == "version") {
        handleVersion(socket, msg, cId, client);
    } else if (ep == "inform") {
        handleInform(socket, msg, cId, client);
    } else if (ep == "register") {
        handleRegister(socket, msg, cId, client);
    } else if (ep == "request") {
        handleRequest(socket, msg, cId, client);
    } else if (ep == "action") {
        handleAction(socket, msg, cId, client);
    } else if (ep == "poll") {
        handlePoll(socket, msg, cId, client);
    } else {
        qWarning() << "Resource not found: " << ep;
        HttpResponse r(HttpResponse::Code::NotFound);
        r.addCorsHeaders();
        reply(r);
    }
}

void HttpServer::handleVersion(QTcpSocket *socket, Message msg, const QString &cId, HttpClientConnection *client)
{
    Q_UNUSED(cId);
    Q_UNUSED(client);

    if (msg.type != Message::Type::C_VERSION) {
        qWarning() << "Received invalid message on EP version: " << Message::msg_to_str(msg.type);
        return;
    }

    // Version is independed of client, handle in pool thread
    quint32 vers;
    msg.payload >> vers;

    sendMessage(socket, Message::Type::S_VERSION, QVariant(Message::PROTOCOL_VERSION));
    if (vers != Message::PROTOCOL_VERSION) {
        // noop, closing does nothing
    }
}

void HttpServer::handleInform(QTcpSocket *socket, Message msg, const QString &cId, HttpClientConnection *client)
{
    Q_UNUSED(cId);
    Q_UNUSED(client);

    switch (msg.type) {
    case Message::Type::C_SET_AUTOSKIP:
    case Message::Type::C_CONFIRM_ACTIVE:
    case Message::Type::C_ACKNOWLEDGED:
        break;
    default:
        qWarning() << "Received invalid message on EP inform: " << Message::msg_to_str(msg.type);
        sendMessage(socket, HttpResponse::Code::BadRequest, QString("Invalid message for EP inform").toLatin1());
        return;
    }

    // this will have no response, immediately send HTTP OK
    // handle in client thread, without setting new response socket
    handleMessageOnClient(client, nullptr, msg, false);
    sendMessage(socket, QByteArray());
}

void HttpServer::handleRegister(QTcpSocket *socket, Message msg, const QString &cId, HttpClientConnection *client)
{
    Q_UNUSED(cId);
    Q_UNUSED(client);

    if (msg.type != Message::Type::C_REGISTER_CLIENT) {
        qWarning() << "Received invalid message on EP register: " << Message::msg_to_str(msg.type);
        sendMessage(socket, Message::INVALID_MESSAGE, QString("Expected C_REGISTER_CLIENT"));
        return;
    }

    // Create a new client and thread
    QUuid newId = QUuid::createUuid();
    HttpClientConnection *c = new HttpClientConnection(newId);
    c->setActiveSocket(socket);
    ClientConnection *conn = new ClientConnection(c);
    c->setClientConnection(conn);
    conn->setParent(c);
    //socket->setParent(c);

    AH::Common::CleanupThread *t = new AH::Common::CleanupThread;
    socket->moveToThread(t);
    c->moveToThread(t);
    connect(t, SIGNAL(started()), c, SLOT(startup()));
    connect(t, SIGNAL(finished()), c, SLOT(deleteLater()));
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(c, SIGNAL(shutdown()), t, SLOT(quit()));
    t->registerCleanupMethod(c, "cleanup");
    t->start();

    m_clients[newId] = c;
}

void HttpServer::handleRequest(QTcpSocket *socket, Message msg, const QString &cId, HttpClientConnection *client)
{
    Q_UNUSED(cId);
    Q_UNUSED(client);

    if (msg.type == Message::C_REQUEST_OBJECTS) {
        // Direct handling of describe requests
        client->handleMessageDirect(msg, socket);
    } else {
        qWarning() << "Received invalid message on EP request: " << Message::msg_to_str(msg.type);
        sendMessage(socket, Message::Type::INVALID_MESSAGE, QString("Expected C_REQUEST_OBJECTS"), true);
    }
}

void HttpServer::handleAction(QTcpSocket *socket, Message msg, const QString &cId, HttpClientConnection *client)
{
    Q_UNUSED(cId);

    switch (msg.type) {
    case Message::Type::C_VERSION:
    case Message::Type::C_ACKNOWLEDGED:
    case Message::Type::C_CONFIRM_ACTIVE:
    case Message::Type::C_REGISTER_CLIENT:
    case Message::Type::C_REQUEST_OBJECTS:
        qWarning() << "Received invalid message on EP action: " << Message::msg_to_str(msg.type);
        sendMessage(socket, Message::Type::INVALID_MESSAGE, QString("Did not expect %1 on EP action").arg(Message::msg_to_str(msg.type)), true);
        return;
    default:
        break;
    }

    handleMessageOnClient(client, socket, msg, true);
}

void HttpServer::handlePoll(QTcpSocket *socket, Message msg, const QString &cId, HttpClientConnection *client)
{
    Q_UNUSED(msg);
    Q_UNUSED(cId);
    Q_UNUSED(client);

    client->handleLongPoll(socket);
}

void HttpServer::sendMessages(QTcpSocket *socket, QList<Message> msgs, bool closeSocket)
{
    QJsonArray a;
    for (auto m : msgs) {
        QVariant v;
        v << m;
        qDebug() << "Sending Message " << AH::Common::Message::msg_to_str(m.type);
        QJsonObject o = QJsonObject::fromVariantMap(v.toMap());
        a.append(o);
    }
    QJsonDocument d;
    d.setArray(a);
    QByteArray ba = d.toJson(QJsonDocument::JsonFormat::Compact);

    sendMessage(socket, ba, closeSocket);
}

void HttpServer::sendMessage(QTcpSocket *socket, Message msg, bool closeSocket)
{
    QVariant v;
    v << msg;
    QJsonDocument d = QJsonDocument::fromVariant(v);
    QByteArray data = d.toJson(QJsonDocument::JsonFormat::Compact);

    qDebug() << "Sending Message " << AH::Common::Message::msg_to_str(msg.type);
    sendMessage(socket, data, closeSocket);
}

void HttpServer::sendMessage(QTcpSocket *socket, Message::Type type, QVariant payload, bool closeSocket)
{
    Message msg;
    msg.type = type;
    msg.payload = payload;
    sendMessage(socket, msg, closeSocket);
}

void HttpServer::sendMessage(QTcpSocket *socket, QByteArray data, bool closeSocket)
{
    sendMessage(socket, HttpResponse::Code::Ok, data, closeSocket);
}

void HttpServer::sendMessage(QTcpSocket *socket, HttpResponse::Code code, QByteArray data, bool closeSocket)
{
    HttpResponse res(code);
    res.addCorsHeaders();
    res.setContent(data);
    res.writeTo(socket);
    socket->flush();
    if (closeSocket) {
        socket->close();
        //qDebug() << "Thread " << QThread::currentThreadId() << " socket " << socket << ": sendMessage DELETE";
        delete socket;
    }
}

HttpClientConnection *HttpServer::getClient(const QString &cId) const
{
    return m_clients[cId];
}

void HttpServer::handleMessageOnClient(HttpClientConnection *client, QTcpSocket *socket, Message msg, bool flush)
{
    if (socket && socket->thread() == QThread::currentThread()) {
        socket->moveToThread(client->thread());
    }

    client->handToClientThread(socket, [=](){
        // This will post the message to game thread
        client->handleMessage(msg);
        if (flush) {
            // maybe game thread can handle and already answer, so wait a bit (yield)
            QThread::yieldCurrentThread();
            client->flush();
        }
    });
}

HttpClientConnection::HttpClientConnection(QUuid clientId)
    : m_clientId(clientId)
    , m_socket(nullptr)
    , m_directSocket(nullptr)
    , m_clientConnection(nullptr)
    , m_mut(QReadWriteLock::Recursive)
    , m_activeLongPollTimer(0)
{

}

void HttpClientConnection::startup()
{

    // Make fake version message
    Message m;
    m.type = Message::C_VERSION;
    m.payload << Message::PROTOCOL_VERSION;
    handleMessage(m);

    //sendMessage(socket, Message::S_REGISTER_CLIENT, newId);
    sendMessage(Message::S_REGISTER_CLIENT, m_clientId);
    flush();
}

void HttpClientConnection::sendMessage(const Message &msg)
{
    // just add to pending
    QWriteLocker wl(&m_mut);
    m_pendingQueue << msg;
    qDebug() << "- Pending Message " << AH::Common::Message::msg_to_str(msg.type);
    wl.unlock();
    flushLongPoll();
}

void HttpClientConnection::sendMessage(const Message &msg, intptr_t msgId)
{
    if (m_directSocket && msgId == reinterpret_cast<intptr_t> (m_directSocket)) {
        HttpServer::sendMessage(m_directSocket, msg, false);
        return;
    }
    sendMessage(msg);
}

bool HttpClientConnection::hasActiveSocket() const
{
    QReadLocker rl(&m_mut);
    return m_socket != nullptr && m_socket->isOpen();
}

void HttpClientConnection::setActiveSocket(QTcpSocket *socket)
{
    //qDebug() << "Thread " << QThread::currentThreadId() << " socket " << socket << ": setActiveSocket";
    flushQueue();
    replaceSocket(socket);
}

void HttpClientConnection::flush()
{
    bool hasData = false;
    {
        QReadLocker rl(&m_mut);
        hasData = m_pendingQueue.length() > 0;
    }
    if (hasData) {
        // move pending to flush queue
        QWriteLocker wl(&m_mut);
        m_flushQueue.append(m_pendingQueue);
        m_pendingQueue.clear();
    }

    flushQueue();
}

void HttpClientConnection::handleMessage(Message msg)
{
    // This may post to game thread
    emit messageReceived(msg);
}

void HttpClientConnection::handleMessageDirect(Message msg, QTcpSocket *socket)
{
    socket->moveToThread(thread());
    // hand to client thread, but don't set new response socket
    handToClientThread(nullptr, [=]() {
        // The message Id ensures that only responses for this request
        // are sent back on this socket.

        // Protect agains multiple parallel requests
        QWriteLocker wl(&m_directMut);
        m_directSocket = socket;
        emit messageReceivedWithId(msg, reinterpret_cast<intptr_t>(socket));
        m_directSocket = nullptr;
        socket->flush();
        socket->close();
        delete socket;
    });
}

void HttpClientConnection::setClientConnection(ClientConnection *c)
{
    if (m_clientConnection) {
        qFatal("HttpClientConnection: Already have client connection");
    }
    m_clientConnection = c;
}

void HttpClientConnection::replaceSocket(QTcpSocket *socket)
{
    //qDebug() << "Thread " << QThread::currentThreadId() << " socket " << socket << ": replaceSocket from " << m_socket;
    QTcpSocket *oldSocket = nullptr;

    { // lock scope
        QWriteLocker wl(&m_mut);
        if (socket == m_socket)
            return;

        // We replace the poll socket, so stop timer
        cancelLongPoll();

        oldSocket = m_socket;
        m_socket = socket;
        //qDebug() << "Thread " << QThread::currentThreadId() << " setting socket to " << m_socket;
        if (m_socket && m_socket->thread() != this->thread()) {
            qCritical() << "Setting new socket that lives in other thread!";
        }
    }

    if (oldSocket) {
        if (oldSocket->isOpen()) {
            oldSocket->flush();
            oldSocket->close();
        }
        //qDebug() << "Thread " << QThread::currentThreadId() << " socket " << oldSocket << ": replaceSocket DELETE";
        delete oldSocket;
    }
}

void HttpClientConnection::flushQueue()
{
    if (QThread::currentThread() == this->thread()) {
        doFlushQueue();
    } else {
        QMetaObject::invokeMethod(this,
                "doFlushQueue",
                Qt::BlockingQueuedConnection);
    }
}

void HttpClientConnection::doFlushQueue()
{
    //qDebug() << "Thread " << QThread::currentThreadId() << " socket " << m_socket << ": doFlushQueue";
    QReadLocker rl(&m_mut);
    if (hasActiveSocket()) {
        // Make local copy of flushed messages
        QList<Message> toWrite;
        if (m_flushQueue.size() > 0) {
            rl.unlock();
            QWriteLocker wl(&m_mut);
            toWrite = m_flushQueue;

            // We send data now, so long polling will be finished
            cancelLongPoll();
            m_flushQueue.clear();

            // Write out flushed messages
            if (toWrite.length() > 0) {
                HttpServer::sendMessages(m_socket, toWrite, false);
            }

            replaceSocket(nullptr);
        }
    } else {
        // have a socket, but it's closed... delete
        //qDebug() << "Thread " << QThread::currentThreadId() << " Flush: Have no socket!";
        rl.unlock();
        replaceSocket(nullptr);
    }
}

void HttpClientConnection::setSocketAndExec(QTcpSocket *socket, HttpClientConnection::VoidFunc f)
{
    //qDebug() << "Thread " << QThread::currentThreadId() << " socket " << socket << ": setSocketAndExec";
    if (socket)
        setActiveSocket(socket);
    f();
}

void HttpClientConnection::doHandleLongPoll(QTcpSocket *socket)
{
    //qDebug() << "Thread " << QThread::currentThreadId() << " entering doHandleLongPoll";
    setActiveSocket(socket);
    QReadLocker rl(&m_mut);
    if (m_pendingQueue.length() > 0) {
        //qDebug() << "Thread " << QThread::currentThreadId() << " pending messages, flushing pending";
        rl.unlock();
        flush();
        return;
    }
    if (m_flushQueue.length() > 0) {
        //qDebug() << "Thread " << QThread::currentThreadId() << " pending messages, flushing flush";
        rl.unlock();
        flushQueue();
        return;
    }
    if (m_socket) {
        // No data to write, we still have the socket.
        // Wait for a timeout
        rl.unlock();

        //qDebug() << "Thread " << QThread::currentThreadId() << " starting poll timer";
        startLongPoll();
    }
}

void HttpClientConnection::startLongPoll()
{
    QWriteLocker wl(&m_mut);
    cancelLongPoll();
    m_activeLongPollTimer = QObject::startTimer(10000);
}

void HttpClientConnection::cancelLongPoll()
{
    QWriteLocker wl(&m_mut);
    if (m_activeLongPollTimer != 0) {
        QObject::killTimer(m_activeLongPollTimer);
        m_activeLongPollTimer = 0;
    }
}

void HttpClientConnection::flushLongPoll()
{
    if (QThread::currentThread() == this->thread()) {
        doFlushLongPoll();
    } else {
        QMetaObject::invokeMethod(this,
                "doFlushLongPoll",
                Qt::QueuedConnection
                );
    }
}

void HttpClientConnection::doFlushLongPoll()
{
    QReadLocker rl(&m_mut);
    //qDebug() << "Thread " << QThread::currentThreadId() << " entering flushLongPoll";
    if (m_activeLongPollTimer != 0) {
        // give it some time to maybe accumulate multiple messages
        // this may be a bit problematic, as the a new connection might come in.
        // protect against that. If this happens, a new poll will get the data immediately
        QTcpSocket *pollSocket = m_socket;

        rl.unlock();
        cancelLongPoll();

        //qDebug() << "Thread " << QThread::currentThreadId() << " setting timer for flush for poll";

        if (pollSocket != nullptr) {
            QTimer::singleShot(100, this, [=](){
                //qDebug() << "Thread " << QThread::currentThreadId() << " flushing from poll " << m_pendingQueue.length();
                if (pollSocket == m_socket)
                    flush();
            });
        }
    } else {
        //qDebug() << "Thread " << QThread::currentThreadId() << " no active poll";
    }
}

void HttpClientConnection::handleLongPoll(QTcpSocket *socket)
{
    if (QThread::currentThread() == this->thread()) {
        doHandleLongPoll(socket);
    } else {
        socket->moveToThread(thread());
        QMetaObject::invokeMethod(this,
                "doHandleLongPoll",
                Qt::QueuedConnection,
                Q_ARG(QTcpSocket *, socket)
                );
    }
}

void HttpClientConnection::handToClientThread(QTcpSocket *socket, HttpClientConnection::VoidFunc f)
{
    if (QThread::currentThread() == this->thread()) {
        setSocketAndExec(socket, f);
    } else {
        QMetaObject::invokeMethod(this,
                "setSocketAndExec",
                Qt::BlockingQueuedConnection,
                Q_ARG(QTcpSocket *, socket),
                Q_ARG(HttpClientConnection::VoidFunc, f));
    }
}

void HttpClientConnection::timerEvent(QTimerEvent *ev)
{
    QReadLocker rl(&m_mut);
    if (ev->timerId() == m_activeLongPollTimer) {
        ev->accept();
        rl.unlock();

        QWriteLocker wl(&m_mut);
        cancelLongPoll();
        bool hasData = m_pendingQueue.length() > 0;
        wl.unlock();

        if (!hasData)
            sendMessage(AH::Common::Message::NO_MESSAGE, QVariant());
        flush();
        return;
    } else {
        rl.unlock();
        INetworkConnection::timerEvent(ev);
    }
}
