#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>
#include <QUuid>
#include <QMap>
#include <QList>
#include <QReadWriteLock>
#include <communication/message.h>
#include <communication/inetworkconnection.h>
#include <communication/httpparser.h>
#include "clientconnection.h"

class ClientConnection;

class HttpClientConnection : public AH::Common::INetworkConnection
{
    Q_OBJECT
public:
    HttpClientConnection(QUuid clientId);

    virtual void startup() override;
    //virtual void stop() override {};
    //virtual void cleanup() override {};
    //virtual void close() override {};
    //virtual void abort() override {};
    virtual void flush() override;

    virtual void sendMessage(const AH::Common::Message &msg) override;
    virtual void sendMessage(const AH::Common::Message &msg, intptr_t msgId) override;
    using INetworkConnection::sendMessage;

    QUuid clientId() const { return m_clientId; }
    bool hasActiveSocket() const;
    void setActiveSocket(QTcpSocket *socket);

    void handleMessage(AH::Common::Message msg);

    void handleMessageDirect(AH::Common::Message msg, QTcpSocket *socket);

    void setClientConnection(ClientConnection *c);
    ClientConnection *clientConnection() { return m_clientConnection; }
    const ClientConnection *clientConnection() const { return m_clientConnection; }


    using VoidFunc = std::function<void()>;
    //void handToClientThread(HttpClientConnection::VoidFunc f);
    void handToClientThread(QTcpSocket *socket, HttpClientConnection::VoidFunc f);

    void handleLongPoll(QTcpSocket *socket);

protected:
    void timerEvent(QTimerEvent *ev) override;

private:
    void replaceSocket(QTcpSocket *socket);
    void flushQueue();
    Q_INVOKABLE void doFlushQueue();


    Q_INVOKABLE void setSocketAndExec(QTcpSocket *socket, HttpClientConnection::VoidFunc f);
    Q_INVOKABLE void doHandleLongPoll(QTcpSocket *socket);

    void startLongPoll();
    void cancelLongPoll();
    void flushLongPoll();
    Q_INVOKABLE void doFlushLongPoll();

private:
    QUuid m_clientId;
    // Current response socket
    QTcpSocket *m_socket;
    // Socket for direct response (e.g. request object)
    QTcpSocket *m_directSocket;
    ClientConnection *m_clientConnection;
    // List of pending messages to return
    QList<AH::Common::Message> m_pendingQueue;
    // List of messages that were flushed, but not written (no socket available)
    QList<AH::Common::Message> m_flushQueue;
    mutable QReadWriteLock m_mut;
    QReadWriteLock m_directMut;
    int m_activeLongPollTimer;
};

class HttpServer : public QTcpServer
{
    Q_OBJECT
public:
    HttpServer();
    static constexpr quint16 ServerPort = 80;

public slots:
    void start();
    void stop();

protected:
    void incomingConnection ( qintptr socketDescriptor ) override;

private:
    void handleConnection(QTcpSocket *socket);

    void handleVersion(QTcpSocket *socket, AH::Common::Message msg, const QString &cId, HttpClientConnection *client);
    void handleInform(QTcpSocket *socket, AH::Common::Message msg, const QString &cId, HttpClientConnection *client);
    void handleRegister(QTcpSocket *socket, AH::Common::Message msg, const QString &cId, HttpClientConnection *client);
    void handleRequest(QTcpSocket *socket, AH::Common::Message msg, const QString &cId, HttpClientConnection *client);
    void handleAction(QTcpSocket *socket, AH::Common::Message msg, const QString &cId, HttpClientConnection *client);
    void handlePoll(QTcpSocket *socket, AH::Common::Message msg, const QString &cId, HttpClientConnection *client);

    static void sendMessages(QTcpSocket *socket, QList<AH::Common::Message> msgs, bool closeSocket = true);
    static void sendMessage(QTcpSocket *socket, AH::Common::Message msg, bool closeSocket = true);
    static void sendMessage(QTcpSocket *socket, AH::Common::Message::Type type, QVariant payload, bool closeSocket = true);
    static void sendMessage(QTcpSocket *socket, QByteArray data, bool closeSocket = true);
    static void sendMessage(QTcpSocket *socket, AH::Common::HttpResponse::Code code, QByteArray data, bool closeSocket = true);

    HttpClientConnection *getClient(const QString &cId) const;

    void handleMessageOnClient(HttpClientConnection *client, QTcpSocket *socket, AH::Common::Message msg, bool flush);

private:
    using ClientMap = QMap<QUuid, HttpClientConnection *>;
    ClientMap m_clients;
    friend class HttpClientConnection;
};



#endif // HTTPSERVER_H
