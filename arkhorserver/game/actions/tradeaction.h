#ifndef TRADEACTION_H
#define TRADEACTION_H

#include "../gameaction.h"
#include "../gameoption.h"
#include <QStringList>
#include <tradedata.h>

class Player;
class Character;
class GameObject;

class TradeAction : public GameAction
{
public:
    TradeAction();

    bool execute() override;
    virtual QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    AH::GamePhases phases() const override { return AH::Movement; }

    QString id() const override { return "AC_TRADE"; }
    QString name() const override { return "Trade"; }
    QString description() const override { return "Trade items with another player"; }

    static QList<Character *> getTradingPartners(Character *c);

private:
    bool doTrade(Character *partner);
    Character *chooseTradingPartner(Player *p, const QList<Character*> &partners);
    QStringList getTradables(Character *c);
    AH::Common::TradeData getTradeOffer(Character *c, AH::Common::TradeData td);

    void exchange(Character *from, Character *to, QStringList items);
    bool getMoney(Character *c, int amount);
    GameObject *getObject(Character *c, QString id);
};

class TradeOption : public GameOption
{
public:
    TradeOption() : GameOption(&ta, AH::CanContinue, AH::ChooseSupplemental, AH::Movement)
    {}

    QString id() const override { return "OP_TRADE"; }

    virtual bool isAvailable() const override;

private:
    TradeAction ta;
};

#endif // TRADEACTION_H
