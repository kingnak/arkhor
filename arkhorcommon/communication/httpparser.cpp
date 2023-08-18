#include "httpparser.h"
#include <QDebug>
#include <QString>
#include <QEventLoop>
#include <QTimer>

namespace AH { namespace Common {

HttpRequest HttpRequest::readMessage(QIODevice *dev, int timeout)
{
    HttpRequest res;

    auto waitForContent = [timeout](QIODevice *dev) {
        if (timeout > 0 && dev->bytesAvailable() == 0) {
            // Qt sais waitForReadyRead may randomly fail on windows, and use an eventloop instead...
            QEventLoop l;
            auto c = QObject::connect(dev, &QIODevice::readyRead, &l, &QEventLoop::quit);
            QTimer::singleShot(timeout, &l, &QEventLoop::quit);
            l.exec();
            QObject::disconnect(c);
            //dev->waitForReadyRead(timeout);
        }
    };

    auto readLine = [waitForContent](QIODevice *dev)->QByteArray {
        waitForContent(dev);
        auto b = dev->readLine();
        if (b.endsWith("\r\n")) b = b.left(b.length()-2);
        return b;
    };


    QByteArray b = readLine(dev);
    if (b.startsWith("GET ")) {
        res.m_method = Method::GET;
    } else if (b.startsWith("POST ")) {
        res.m_method = Method::POST;
    } else if (b.length() == 0){
        return res.makeError("Connection Timeout", HttpResponse::Code::RequestTimeout);
    } else {
        return res.makeError("Method not supported", HttpResponse::Code::MethodNotAllowed);
    }

    int sp1 = b.indexOf(' ');
    int sp2 = b.lastIndexOf(' ');
    if (sp1 < 0 || sp2 <= sp1) {
        return res.makeError("Invalid Request line");
    }

    res.m_resource = QString::fromUtf8(b.mid(sp1+1, sp2 - sp1-1));
    QString v = QString::fromLatin1(b.mid(sp2+1));
    if (v != "HTTP/1.1") {
        return res.makeError("Unsupported HTTP version: " + v, HttpResponse::Code::HttpVersionNotSupported);
    }

    b = readLine(dev);
    while (b.length() > 0) {
        int cpos = b.indexOf(':');
        if (cpos < 0) {
            return res.makeError("Invalid header: " + b);
        }

        QString k = QString::fromUtf8(b.left(cpos));
        QString v = QString::fromUtf8(b.mid(cpos+1)).trimmed();
        res.m_headers[k] = v;
        b = readLine(dev);

        // TODO: Handle too large data?
    }

    const auto &h = res.m_headers;
    auto e = h["Content-Encoding"];
    if (e.length() > 0 && e != "identity") {
        return res.makeError("Content-Encoding not supported: " + e);
    }

    e = h["Content-Length"];
    quint32 clen = 0;
    if (e.length() > 0) {
        bool ok;
        clen = e.toUInt(&ok);
        if (!ok) {
            return res.makeError("Invalid content length: " + e);
        }
    }

    e = h["Transfer-Encoding"];
    if (e.length() > 0 && e != "identity") {
        return res.makeError("Transfer-Encoding not supported: " + e);
    }

    if (clen > 0) {
        if (res.m_method == Method::GET)
            return res.makeError("GET cannot have content");

        if (clen > MaxContentLength) {
            return res.makeError(QString("Content too large (max %1 bytes)").arg(MaxContentLength), HttpResponse::Code::PayloadTooLarge);
        }

        auto &c = res.m_content;
        waitForContent(dev);
        while (clen > 0 && !dev->atEnd()) {
            waitForContent(dev);
            b = dev->read(clen);
            c += b;
            clen -= b.length();

            // TODO: Handle waiting for data?
        }

        if (clen > 0) {
            return res.makeError("Cannot read whole content");
        }
    } else {
        if (res.m_method == Method::POST && !h.contains("Content-Length")) {
            return res.makeError("No Length in POST", HttpResponse::Code::LengthRequired);
        }
    }

    return res;
}

HttpResponse HttpRequest::createResponseFromError() const
{
    HttpResponse res(m_response);
    res.addCorsHeaders();
    res.setContent(m_error.toLatin1());
    return res;
}

HttpRequest HttpRequest::makeError(QString error, HttpResponse::Code code)
{
    HttpRequest r(*this);
    r.m_error = error;
    r.m_response = code;
    return r;
}

HttpResponse::HttpResponse(HttpResponse::Code c)
    : m_code(c)
{
}

void HttpResponse::addHeader(QString k, QString v)
{
    m_headers[k] = v;
}

void HttpResponse::setContent(QByteArray data, bool addLengthHeader)
{
    m_content = data;
    if (addLengthHeader) {
        addHeader("Content-Length", QString::number(data.length()));
    }
}

void HttpResponse::addCorsHeaders()
{
    addHeader("Access-Control-Allow-Origin", "*");
    addHeader("Access-Control-Allow-Headers", "x-requested-with");
}

bool HttpResponse::writeTo(QIODevice *dev)
{
    QByteArray b;
    b += QString("HTTP/1.1 %1 %2\r\n")
            .arg(static_cast<int>(m_code))
            .arg(messageForCode(m_code))
            .toLatin1()
            ;

    for (const auto it : m_headers.toStdMap()) {
        b += QString("%1: %2\r\n").arg(it.first, it.second).toLatin1();
    }

    b += "\r\n";

    b += m_content;

    auto written = dev->write(b);
    return written == b.length();
}

QString HttpResponse::messageForCode(HttpResponse::Code code)
{
    switch (code)
    {
    case Code::Ok: return QString("Ok");
    case Code::BadRequest: return QString("Bad request");
    case Code::NotFound: return QString("Not found");
    case Code::MethodNotAllowed: return QString("Method not allowed");
    case Code::NotAcceptable: return QString("Not acceptable");
    case Code::RequestTimeout: return QString("Request timeout");
    case Code::LengthRequired: return QString("Length required");
    case Code::PayloadTooLarge: return QString("Payload too large");
    case Code::UnsupportedMediaType: return QString("Unsupported media type");
    case Code::ServerError: return QString("Internal server error");
    case Code::HttpVersionNotSupported: return QString("HTTP version not supported");
    default: return QString("Unkown");
    }
}



}}
