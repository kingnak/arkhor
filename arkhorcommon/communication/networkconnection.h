#ifndef NETWORKCONNECTION_H
#define NETWORKCONNECTION_H

#include "arkhorcommonglobal.h"
#include "inetworkconnection.h"
#include <QObject>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>
#include "message.h"

namespace AH {
    namespace Common {

        class ARKHOR_COMMON_EXPORTS NetworkConnection : public INetworkConnection // : public QTcpSocket
        {
            Q_OBJECT
        public:
            explicit NetworkConnection(qintptr socketDescriptor, QObject *parent = 0);
            explicit NetworkConnection(QTcpSocket *sock, QObject *parent = 0);

            void setPinging() override { m_doPing = true; startPinging(); }

        public slots:
            virtual void startup() override;
            virtual void stop() override;
            virtual void cleanup() override;
            virtual void close() override;
            virtual void abort() override;

        public:
            virtual void sendMessage(const AH::Common::Message &msg) override;
            using INetworkConnection::sendMessage;

        protected:
            virtual void receivedMessage(const Message &msg);
            void timerEvent(QTimerEvent *timerEvent) override;

        private slots:
            void startPinging();
            void sendPing();
            void dataReady();

        private:
            Q_INVOKABLE void doSendMessage(const AH::Common::Message &msg);
            Q_INVOKABLE void doClose();
            Q_INVOKABLE void doAbort();

            void startTransferTimeout();
            void stopTransferTimeout();

            qint64 readDataIntoBuffer(qint64 maxSize);
            bool readProtocolHeader();
            bool readLength();
            bool hasEnoughData();
            void processMessage();
            void handleMessage(const Message &msg);

        private:
            template <typename T, size_t N>
            static void intToBytes(T val, char(&dest)[N]);
            template <typename T>
            static void intToBytes(T val, char **dest, size_t N);
            template <typename T, size_t N>
            static T byteToInt(const char (&src)[N]);
            template<typename T>
            static T byteToInt(const char *src, size_t N);

        private:
            qintptr m_sockDesc;
            QTcpSocket *m_socket;
            QByteArray m_buffer;

            enum {
                ReadHeader,
                ReadLength,
                ReadMessage,
                Error
            } m_state;

            quint32 m_lengthNeeded;

            QTime m_pongTime;
            QTimer *m_pingTimer;
            int m_transferTimer;
            bool m_doPing;
            static const int PONG_TIMEOUT = 60 * 1000;
            static const int PING_TIMEOUT = 10 * 1000;
            static const int TRANSFER_TIMEOUT = 30 * 1000;

            static const quint32 MAGICK = 0x782710ab;
            static const quint32 MAGICK_SIZE = sizeof(MAGICK);
            static const qint32 LENGTH_SIZE = sizeof(quint32);
            static const qint32 HEADER_SIZE = MAGICK_SIZE + LENGTH_SIZE;

        };
    }
}

#endif // NETWORKCONNECTION_H
