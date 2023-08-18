#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <QObject>
#include <QUuid>
#include <communication/inetworkconnection.h>

class QNetworkAccessManager;
class QNetworkReply;

class HttpConnection : public AH::Common::INetworkConnection
{
    Q_OBJECT
public:
    explicit HttpConnection(QString host, quint16 port, QObject *parent = nullptr);

public slots:
    virtual void startup() override;
    //virtual void stop() override;
    //virtual void cleanup() override;
    //virtual void close() override;
    //virtual void abort() override;

public:
    virtual void sendMessage(const AH::Common::Message &msg) override;
    using INetworkConnection::sendMessage;

protected:
    void timerEvent(QTimerEvent *ev) override;

private slots:
    void received(QNetworkReply *reply);
    void handleMessage(AH::Common::Message msg);

private:
    void startPoll();

private:
    static bool messageHasData(AH::Common::Message::Type type);
    static QString messageToEP(AH::Common::Message::Type type);
    static bool messageHasClientId(AH::Common::Message::Type type);

private:
    QString m_host;
    quint16 m_port;
    QUuid m_clientId;
    QNetworkAccessManager *m_mgm;
    bool m_versionHandled;
    int m_pollTimerId;
    QNetworkReply *m_pollReply;
};

#endif // HTTPCONNECTION_H
