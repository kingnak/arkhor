#ifndef FAKEGAMEOPTION_H
#define FAKEGAMEOPTION_H

#include "gameaction.h"
#include "gameoption.h"

class FakeGameAction : public GameAction
{
public:
    FakeGameAction(QString id, QString name, QString description);

    virtual bool execute() { return true; }
    virtual QString notificationString(NotificationPart part, const QString &desc = QString::null) const { return {}; }

};

class FakeGameOption : public GameOption
{
public:
    FakeGameOption(QString id, QString name, QString description);
    bool isAvailable() const { return false; }

private:
    FakeGameAction fga;
};

#endif  // FAKEGAMEOPTION_H
