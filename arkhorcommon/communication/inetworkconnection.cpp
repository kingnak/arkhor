#include "inetworkconnection.h"

namespace AH { namespace Common {

INetworkConnection::INetworkConnection(QObject *parent)
    : QObject(parent)
{

}

void INetworkConnection::sendMessage(Message::Type type, const QVariant &payload)
{
    Message msg;
    msg.type = type;
    msg.payload = payload;
    sendMessage(msg);
}

void INetworkConnection::sendMessage(const Message &msg, intptr_t msgId)
{
    Q_UNUSED(msgId);
    sendMessage(msg);
}

void INetworkConnection::sendMessage(Message::Type type, const QVariant &payload, intptr_t msgId)
{
    Message msg;
    msg.type = type;
    msg.payload = payload;
    sendMessage(msg, msgId);
}

}}
