#include "httpconnection.h"
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QTimerEvent>

HttpConnection::HttpConnection(QString host, quint16 port, QObject *parent)
    : AH::Common::INetworkConnection(parent)
    , m_host(host)
    , m_port(port)
    , m_versionHandled(false)
    , m_pollTimerId(0)
    , m_pollReply(nullptr)
{
    m_mgm = new QNetworkAccessManager(this);
    connect(m_mgm, &QNetworkAccessManager::finished, this, &HttpConnection::received);
}

void HttpConnection::startup()
{
    sendMessage(AH::Common::Message::Type::C_REGISTER_CLIENT, QVariant());
}

void HttpConnection::sendMessage(const AH::Common::Message &msg)
{
    if (m_pollReply) {
        m_pollReply = nullptr;
    }

    QVariant v;
    v << msg;

    auto ep = QString("http://%2:%1/").arg(m_port).arg(m_host);
    ep += messageToEP(msg.type);
    if (messageHasClientId(msg.type)) ep += "/" + m_clientId.toString(QUuid::StringFormat::WithoutBraces);
    if (messageHasData(msg.type)) {
        auto data = QJsonDocument::fromVariant(v).toJson(QJsonDocument::JsonFormat::Indented);
        QNetworkRequest req = QNetworkRequest(QUrl(ep));
        req.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "text/json");
        m_mgm->post(req, data);
    } else {
        m_mgm->get(QNetworkRequest(QUrl(ep)));
    }

    startPoll();
}

void HttpConnection::timerEvent(QTimerEvent *ev)
{
    if (ev->timerId() == m_pollTimerId) {
        ev->accept();
        killTimer(m_pollTimerId);
        m_pollTimerId = 0;
        auto ep = QString("http://%2:%1/poll/%3").arg(m_port).arg(m_host).arg(m_clientId.toString(QUuid::StringFormat::WithoutBraces));
        m_pollReply = m_mgm->get(QNetworkRequest(QUrl(ep)));
        return;
    }
    INetworkConnection::timerEvent(ev);
}

void HttpConnection::received(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply == m_pollReply)
        m_pollReply = nullptr;

    if (reply->error() != QNetworkReply::NetworkError::NoError) {
        qCritical()
                << "Error in HTTP request: "
                << reply->errorString()
                << " for request at "
                << reply->request().url();
        return;
    }

    QByteArray ba;
    while (!reply->atEnd()) {
        if (reply->bytesAvailable() == 0) {
            QEventLoop l;
            auto c = QObject::connect(reply, &QIODevice::readyRead, &l, &QEventLoop::quit);
            QTimer::singleShot(300, &l, &QEventLoop::quit);
            l.exec();
        }
        ba += reply->readAll();
    }

    QJsonParseError err;
    QJsonDocument json = QJsonDocument::fromJson(ba, &err);
    if (err.error != QJsonParseError::ParseError::NoError) {
        qCritical()
                << "Error in HTTP response, invalid JSON: "
                << err.errorString()
                << " for request at "
                << reply->request().url();
        return;
    }

    QVariant v = json.toVariant();

    if (v.type() == QVariant::List) {
        for (auto vv : v.toList()) {
            AH::Common::Message m;
            vv >> m;
            handleMessage(m);
        }
    } else {
        AH::Common::Message m;
        v >> m;
        handleMessage(m);
    }

    startPoll();
}

void HttpConnection::handleMessage(AH::Common::Message m)
{
    if (m.type == AH::Common::Message::S_VERSION) {
        quint32 vers;
        m.payload >> vers;

        // Version mismatch will be handled later in client handler
        if (vers == AH::Common::Message::PROTOCOL_VERSION) {
            if (!m_versionHandled)
                startup();
            m_versionHandled = true;
        }
    }
    if (m.type == AH::Common::Message::S_REGISTER_CLIENT) {
        QVariant v;
        v << m.payload;
        m_clientId = v.toUuid();
        qDebug() << "Setting client id to " << m_clientId;
    }

    emit messageReceived(m);
}

void HttpConnection::startPoll()
{
    if (m_pollTimerId) {
        killTimer(m_pollTimerId);
        m_pollTimerId = 0;
    }

    m_pollTimerId = startTimer(100);
}

bool HttpConnection::messageHasData(AH::Common::Message::Type type)
{
    /*
    switch (type)
    {
    case AH::Common::Message::Type::C_CONFIRM_ACTIVE:
    case AH::Common::Message::Type::C_ACKNOWLEDGED:
        return false;
    case AH::Common::Message::Type::C_VERSION:
    case AH::Common::Message::Type::C_REGISTER_PLAYER:
    case AH::Common::Message::Type::C_REQUEST_OBJECTS:
    case AH::Common::Message::Type::C_SELECT_INVESTIGATOR:
    case AH::Common::Message::Type::C_DIE_ROLL_UPDATE:
    case AH::Common::Message::Type::C_SELECT_OPTION:
    case AH::Common::Message::Type::C_SELECT_SKILL:
    case AH::Common::Message::Type::C_MOVE_PATH:
    case AH::Common::Message::Type::C_SELECT_FOCUS:
    case AH::Common::Message::Type::C_SELECT_WEAPONS:
    case AH::Common::Message::Type::C_CANCEL_WEAPONS:
    case AH::Common::Message::Type::C_SELECT_ENCOUNTER:
    case AH::Common::Message::Type::C_SELECT_MONSTER:
    case AH::Common::Message::Type::C_SELECT_CHOICE:
    case AH::Common::Message::Type::C_CANCEL_CHOICE:
    case AH::Common::Message::Type::C_SET_AUTOSKIP:
    case AH::Common::Message::Type::C_TRADE:
    case AH::Common::Message::Type::C_CANCEL_TRADE:
        return true;
    default:
        qCritical() << "INVALID MESSAGE TYPE " << type;
        return true;
    }
    */
    Q_UNUSED(type);
    return true;
}

QString HttpConnection::messageToEP(AH::Common::Message::Type type)
{
    switch (type)
    {
    case AH::Common::Message::Type::C_VERSION:
        return "version";
    case AH::Common::Message::Type::C_CONFIRM_ACTIVE:
    case AH::Common::Message::Type::C_SET_AUTOSKIP:
    case AH::Common::Message::Type::C_ACKNOWLEDGED:
        return "inform";
    case AH::Common::Message::Type::C_REGISTER_CLIENT:
        return "register";
    case AH::Common::Message::Type::C_REQUEST_OBJECTS:
        return "request";
    case AH::Common::Message::Type::C_REGISTER_PLAYER:
    case AH::Common::Message::Type::C_START_GAME:
    case AH::Common::Message::Type::C_SELECT_INVESTIGATOR:
    case AH::Common::Message::Type::C_DIE_ROLL_UPDATE:
    case AH::Common::Message::Type::C_SELECT_OPTION:
    case AH::Common::Message::Type::C_SELECT_SKILL:
    case AH::Common::Message::Type::C_MOVE_PATH:
    case AH::Common::Message::Type::C_SELECT_FOCUS:
    case AH::Common::Message::Type::C_SELECT_WEAPONS:
    case AH::Common::Message::Type::C_CANCEL_WEAPONS:
    case AH::Common::Message::Type::C_SELECT_ENCOUNTER:
    case AH::Common::Message::Type::C_SELECT_MONSTER:
    case AH::Common::Message::Type::C_SELECT_CHOICE:
    case AH::Common::Message::Type::C_CANCEL_CHOICE:
    case AH::Common::Message::Type::C_TRADE:
    case AH::Common::Message::Type::C_CANCEL_TRADE:
        return "action";
    default:
        qCritical() << "INVALID MESSAGE TYPE " << type;
        return "";
    }
}

bool HttpConnection::messageHasClientId(AH::Common::Message::Type type)
{
    switch (type)
    {
    case AH::Common::Message::Type::C_VERSION:
    case AH::Common::Message::Type::C_REGISTER_CLIENT:
        return false;
    case AH::Common::Message::Type::C_REQUEST_OBJECTS:
    case AH::Common::Message::Type::C_START_GAME:
    case AH::Common::Message::Type::C_REGISTER_PLAYER:
    case AH::Common::Message::Type::C_CONFIRM_ACTIVE:
    case AH::Common::Message::Type::C_SELECT_INVESTIGATOR:
    case AH::Common::Message::Type::C_DIE_ROLL_UPDATE:
    case AH::Common::Message::Type::C_SELECT_OPTION:
    case AH::Common::Message::Type::C_SELECT_SKILL:
    case AH::Common::Message::Type::C_MOVE_PATH:
    case AH::Common::Message::Type::C_SELECT_FOCUS:
    case AH::Common::Message::Type::C_SELECT_WEAPONS:
    case AH::Common::Message::Type::C_CANCEL_WEAPONS:
    case AH::Common::Message::Type::C_SELECT_ENCOUNTER:
    case AH::Common::Message::Type::C_SELECT_MONSTER:
    case AH::Common::Message::Type::C_ACKNOWLEDGED:
    case AH::Common::Message::Type::C_SELECT_CHOICE:
    case AH::Common::Message::Type::C_CANCEL_CHOICE:
    case AH::Common::Message::Type::C_SET_AUTOSKIP:
    case AH::Common::Message::Type::C_TRADE:
    case AH::Common::Message::Type::C_CANCEL_TRADE:
        return true;
    default:
        qCritical() << "INVALID MESSAGE TYPE " << type;
        return false;
    }
}
