#ifndef CLEANUPTHREAD_H
#define CLEANUPTHREAD_H

#include "arkhorcommonglobal.h"
#include <QThread>
#include <QPair>

namespace AH {
    namespace Common {

        class ARKHOR_COMMON_EXPORTS CleanupThread : public QThread
        {
            Q_OBJECT
        public:
            explicit CleanupThread(QObject *parent = 0);
            ~CleanupThread() override;

            void registerCleanupMethod(QObject *obj, const char *slot);

        protected:
            void run();

        signals:

        public slots:

        private:
            QList<QPair<QObject *, QByteArray> > m_slots;
        };
    }
}
#endif // CLEANUPTHREAD_H
