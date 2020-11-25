#include "tradeaction.h"
#include "../game.h"
#include "../player.h"
#include "character.h"
#include <tradedata.h>
#include <choicedata.h>
#include "../gameobject.h"

TradeAction::TradeAction() = default;

bool TradeAction::execute()
{
    Player *p = gGame->context().player();
    Character *c = p->getCharacter();
    auto traders = getTradingPartners(c);
    if (traders.empty()) {
        return false;
    }

    gGame->notifier()->actionStart(this);
    Character *partner = chooseTradingPartner(p, traders);
    doTrade(partner);
    gGame->notifier()->actionFinish(this);

    return true;
}

bool TradeAction::doTrade(Character *partner)
{
    if (!partner) {
        return false;
    }

    Player *p = gGame->context().player();
    Character *c = p->getCharacter();

    // OFFER Origin => Dest
    AH::Common::TradeData td;
    td.originChar = c->id();
    td.destChar = partner->id();
    td.originOffer = getTradables(c);
    td.phase = AH::Common::TradeData::InitialOffer;
    td = getTradeOffer(c, td);
    if (td.phase == AH::Common::TradeData::NoOffer || td.originOffer.empty()) {
        return false;
    }

    // COUNTER Dest => Origin
    td.phase = AH::Common::TradeData::CounterOffer;
    td.destOffer = getTradables(partner);
    td = getTradeOffer(partner, td);
    if (td.phase == AH::Common::TradeData::NoOffer || td.destOffer.empty()) {
        p->tradeCanceled(partner->investigatorData().name());
        return false;
    }

    // ACCEPT Origin
    td.phase = AH::Common::TradeData::AcceptOffer;
    td = getTradeOffer(c, td);
    if (td.phase == AH::Common::TradeData::NoOffer) {
        gGame->playerForCharacter(partner)->tradeCanceled(c->investigatorData().name());
        return false;
    }

    // Exchange stuff
    exchange(c, partner, td.originOffer);
    exchange(partner, c, td.destOffer);

    return true;
}

Character *TradeAction::chooseTradingPartner(Player *p, const QList<Character *> &partners)
{
    AH::Common::ChoiceData cd;
    cd.setSourceId(id());
    cd.setCanCancel(true);
    cd.setType(AH::Common::ChoiceData::ChooseString);
    cd.setDescription("Choose trading partner");
    QList<AH::Common::ChoiceData::OptionData> opts;
    for (auto c : partners) {
        opts << AH::Common::ChoiceData::OptionData(c->id(), c->investigatorData().name());
    }
    cd.setSelectStrings(opts);

    AH::Common::ChoiceResponseData resp = p->offerChoice(cd);
    for (auto c : partners) {
        if (c->id() == resp.toString())
            return c;
    }
    return nullptr;
}

QStringList TradeAction::getTradables(Character *c)
{
    QStringList ret;
    if (c->money() > 0) ret << QString("$:%1").arg(c->money());
    for (auto o : c->inventory()) {
        switch (o->type()) {
        case AH::GameObjectType::Obj_Spell:
        case AH::GameObjectType::Obj_CommonItem:
        case AH::GameObjectType::Obj_UniqueItem:
            ret << o->id();
            break;
        }
    }
    return ret;
}

AH::Common::TradeData TradeAction::getTradeOffer(Character *c, const AH::Common::TradeData &td)
{
    return gGame->playerForCharacter(c)->offerTrade(td);
}

void TradeAction::exchange(Character *from, Character *to, const QStringList &items)
{
    for (const auto &id : items) {
        if (id.startsWith("$:")) {
            int amount = id.midRef(2).toInt();
            if (getMoney(from, amount))
                to->addMoney(amount);
        } else {
            auto o = getObject(from, id);
            if (o)
                to->addToInventory(o);
        }
    }
}

bool TradeAction::getMoney(Character *c, int amount)
{
    if (amount <= 0) return false;
    AH::Common::CostList cost;
    cost << AH::Common::CostItem(AH::Common::CostItem::Pay_Money, amount);
    return c->pay(cost);
}

GameObject *TradeAction::getObject(Character *c, const QString &id)
{
    GameObject *obj = nullptr;
    for (auto o : c->inventory()) {
        if (o->id() == id) {
            obj = o;
        }
    }

    if (obj)
        c->removeFromInventory(obj);
    return obj;
}

QString TradeAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(desc)
    if (part == GameAction::NotificationPart::Execute) {
        return "{C} traded with {B}";
    }
    return {};
}

QList<Character *> TradeAction::getTradingPartners(Character *c)
{
    switch (c->field()->type()) {
    case AH::Common::FieldData::FieldType::Sky:
    case AH::Common::FieldData::FieldType::Outskirts:
    case AH::Common::FieldData::FieldType::SpaceAndTime:
        return {};
    }

    QList<Character*> allChars = c->field()->characters();
    allChars.removeAll(c);
    return allChars;
}

bool TradeOption::isAvailable() const
{
    return !TradeAction::getTradingPartners(gGame->context().player()->getCharacter()).empty();
}
