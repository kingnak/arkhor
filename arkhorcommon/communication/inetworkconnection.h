#ifndef INETWORKCONNECTION_H
#define INETWORKCONNECTION_H

#include "arkhorcommonglobal.h"
#include <QObject>
#include "message.h"

namespace AH {
    namespace Common {
        class ARKHOR_COMMON_EXPORTS INetworkConnection : public QObject
        {
            Q_OBJECT
        public:
            explicit INetworkConnection(QObject *parent = 0);
            virtual void setPinging() {};

        signals:
            void shutdown();
            void messageReceived(AH::Common::Message msg);
            void messageReceivedWithId(AH::Common::Message msg, intptr_t msgId);

        public slots:
            virtual void startup() {};
            virtual void stop() {};
            virtual void cleanup() {};
            virtual void close() {};
            virtual void abort() {};
            virtual void flush() {};

        public:
            virtual void sendMessage(const AH::Common::Message &msg) = 0;
            virtual void sendMessage(AH::Common::Message::Type type, const QVariant &payload);
            virtual void sendMessage(const AH::Common::Message &msg, intptr_t msgId);
            virtual void sendMessage(AH::Common::Message::Type type, const QVariant &payload, intptr_t msgId);

        };
    }
}
#endif // INETWORKCONNECTION_H
