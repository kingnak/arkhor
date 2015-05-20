#include "cleanupthread.h"

namespace AH { namespace Common {

CleanupThread::~CleanupThread()
{
    this->quit();
    this->wait();
}

void CleanupThread::registerCleanupMethod(QObject *obj, const char *slot)
{
    m_slots.append(qMakePair(obj, QByteArray(slot)));
}

void CleanupThread::run()
{
    QThread::run();

    typedef QPair<QObject *, QByteArray> SlotSpec;
    foreach (SlotSpec s, m_slots) {
        QMetaObject::invokeMethod(s.first, s.second.constData(), Qt::DirectConnection);
    }
}

CleanupThread::CleanupThread(QObject *parent) :
    QThread(parent)
{
}



}}
