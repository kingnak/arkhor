#ifndef NOTIFICATIONFORMATTER_H
#define NOTIFICATIONFORMATTER_H

#include <QStringList>
#include "game/gameaction.h"

class NotificationFormatter
{
public:
    NotificationFormatter();

    QString formatActionStart(const GameAction *action, const QString &desc);
    QString formatActionUpdate(const GameAction *action, const QString &desc);
    QString formatActionFinish(const GameAction *action, const QString &desc);
    QString formatActionExecute(const GameAction *action, const QString &desc);
	QString formatSimple(const QString &str, const QString &desc = QString::null);

private:
    QString getBasicActionString(const GameAction *action, const QString &desc, GameAction::NotificationPart part);
    QString formatBasic(QString str, const QString &desc);

private:
    const GameAction *m_curAction;
    QStringList m_bufList;
};

#endif // NOTIFICATIONFORMATTER_H
