#ifndef FAKEGAMEOPTION_H
#define FAKEGAMEOPTION_H

#include "gameaction.h"
#include "gameoption.h"

class FakeGameAction : public GameAction
{
public:
    FakeGameAction(const QString &id, const QString &name, const QString &description);

    bool execute() override { return true; }
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override { return {}; }

};

class FakeGameOption : public GameOption
{
public:
    FakeGameOption(const QString &id, const QString &name, const QString &description);
    bool isAvailable() const override { return false; }

private:
    FakeGameAction fga;
};

#endif  // FAKEGAMEOPTION_H
