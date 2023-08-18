#ifndef HTTPPARSER_H
#define HTTPPARSER_H

#include "arkhorcommonglobal.h"
#include <QIODevice>
#include <QMap>

namespace AH {
    namespace Common {
        class ARKHOR_COMMON_EXPORTS HttpResponse
        {
        public:
            enum class Code {
                Ok = 200,
                BadRequest = 400,
                NotFound = 404,
                MethodNotAllowed = 405,
                NotAcceptable = 406,
                RequestTimeout = 408,
                LengthRequired = 411,
                PayloadTooLarge = 413,
                UnsupportedMediaType = 415,
                ServerError = 500,
                HttpVersionNotSupported = 505

            };

            HttpResponse(Code c);
            using Headers = QMap<QString, QString>;
            void addHeader(QString k, QString v);
            void setContent(QByteArray data, bool addLengthHeader = true);
            void addCorsHeaders();

            bool writeTo(QIODevice *dev);

            static QString messageForCode(Code code);

        private:
            Code m_code;
            Headers m_headers;
            QByteArray m_content;
        };


        class ARKHOR_COMMON_EXPORTS HttpRequest
        {
            HttpRequest() {}
        public:
            static HttpRequest readMessage(QIODevice *dev, int timeout = 500);

            enum class Method {
                INVALID,
                GET,
                POST,
                OTHER
            };

            operator bool() const { return ok(); };

            bool ok() const { return m_response == HttpResponse::Code::Ok; }

            using Headers = QMap<QString, QString>;
            Method method() const { return m_method; };
            QString resource() const { return m_resource; };
            Headers headers() const { return m_headers; };
            QByteArray content() const { return m_content; };
            QString error() const { return m_error; }

            HttpResponse createResponseFromError() const;

        private:
            HttpRequest makeError(QString error, HttpResponse::Code code = HttpResponse::Code::BadRequest);
            static const qint32 MaxContentLength = 1024*4;

        private:
            Method m_method = Method::INVALID;
            QString m_resource;
            Headers m_headers;
            QByteArray m_content;
            QString m_error;
            HttpResponse::Code m_response = HttpResponse::Code::Ok;
        };



    }
}

#endif // HTTPPARSER_H
