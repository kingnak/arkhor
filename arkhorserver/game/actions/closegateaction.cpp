#include "closegateaction.h"
#include "../game.h"
#include "../player.h"
#include "character.h"

using namespace AH::Common;

CloseGateAction::CloseGateAction()
    : m_bSeal(false)
{
}

bool CloseGateAction::execute()
{
    return true;
}

QString CloseGateAction::name() const
{
    if (m_bSeal)
        return "Seal Gate";
    else
        return "Close Gate";
}


bool CloseGateOption::execute()
{
    ca.m_bSeal = false;
    return GameOption::execute();
}

bool CloseGateOption::isAvailable()
{
    const Gate *g = gGame->context().player()->getCharacter()->getExploredGate();
    GameField *f = gGame->context().player()->getCharacter()->field();

    if (g && f->gate() && g == f->gate()) {
        return true;
    }
    return false;
}


bool SealGateOption::execute()
{
    ca.m_bSeal = true;
    return GameOption::execute();
}

Cost SealGateOption::getCosts() const
{
    int amount = gGame->context().getGameProperty(PropertyValue::Game_SealClueCost).finalVal();
    CostList l;
    l << CostItem(CostItem::Pay_Clue, amount);
    Cost c;
    c.addAlternative(l);
    return c;
}

bool SealGateOption::isAvailable()
{
    const Gate *g = gGame->context().player()->getCharacter()->getExploredGate();
    GameField *f = gGame->context().player()->getCharacter()->field();

    if (g && f->gate() && g == f->gate()) {
        return true;
    }
    return false;
}
