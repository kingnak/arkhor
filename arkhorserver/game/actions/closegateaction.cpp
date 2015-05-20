#include "closegateaction.h"
#include "../game.h"
#include "../player.h"
#include "character.h"
#include "game/dietesthelper.h"
#include "gate.h"
#include "die/dierollevaluator.h"

using namespace AH::Common;

CloseGateAction::CloseGateAction()
    : m_bSeal(false)
{
}

bool CloseGateAction::execute()
{
    // TODO: Let user decide on skill!
    DieTestHelper::DieTestSpec spec = DieTestHelper::createSkillTest(gGame->context().player()->getCharacter(), AH::Skill_Fight, gGame->context().gate()->closeAdjustment());
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(gGame->context().player(), spec);
    if (res.boolResult) {
        gGame->context().gate()->close(gGame->context().player()->getCharacter());
        if (m_bSeal) {
            // TODO: Place ancient sign
        }
    }
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
    gGame->context().setGate(gGame->context().player()->getCharacter()->field()->gate());
    return GameOption::execute();
}

bool CloseGateOption::isAvailable()
{
    const Gate *g = gGame->context().player()->getCharacter()->getExploredGate();
    GameField *f = gGame->context().player()->getCharacter()->field();

    if (g && f->gate() && g == f->gate()) {
        return true;
    }

    // TEST
    //return false;
    return true;
}


bool SealGateOption::execute()
{
    ca.m_bSeal = true;
    gGame->context().setGate(gGame->context().player()->getCharacter()->field()->gate());
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
