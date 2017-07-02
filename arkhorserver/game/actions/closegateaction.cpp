#include "closegateaction.h"
#include "../game.h"
#include "../player.h"
#include "character.h"
#include "game/dietesthelper.h"
#include "gate.h"
#include "die/dierollevaluator.h"
#include "chooseskillhelper.h"

using namespace AH::Common;

CloseGateAction::CloseGateAction()
    : m_bSeal(false)
{
}

bool CloseGateAction::execute()
{
    // Let user decide on skill!
    AH::Skill s = ChooseSkillHelper::chooseSkill(gGame->context().player(), QList<AH::Skill>() << AH::Skill_Fight << AH::Skill_Will);
    if (s == AH::NoSkill) {
        // User abort...
        return true;
    }

    DieTestHelper::DieTestSpec spec = DieTestHelper::createSkillTest(name(), gGame->context().gate()->id(), gGame->context().player()->getCharacter(), s, gGame->context().gate()->closeAdjustment());
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(gGame->context().player(), spec);
    if (res.boolResult) {
        GameField *f = gGame->context().gate()->sourceField();
        if (m_bSeal) {
            f->setSealed(true);
        }
        gGame->notifier()->actionExecute(this);
        gGame->closeGate(gGame->context().gate(), gGame->context().player()->getCharacter());
    }
    return true;
}

QString CloseGateAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(desc);
    Q_ASSERT(part == Execute);
    return QString("{C} ") + (m_bSeal ? "sealed" : "closed") + " gate at {F}";
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
    //gGame->context().setGate(gGame->context().player()->getCharacter()->field()->gate());
    return GameOption::execute();
}

QString CloseGateOption::sourceId() const
{
    if (gGame->context().gate())
        return gGame->context().gate()->id();
    return QString::null;
}

bool CloseGateOption::isAvailable() const
{
    //const Gate *g = gGame->context().player()->getCharacter()->getExploredGate();
    //GameField *f = gGame->context().player()->getCharacter()->field();
    const Gate *eg = gGame->context().player()->getCharacter()->getExploredGate();
    const Gate *fg = gGame->context().gate();

    //if (g && f->gate() && g == f->gate()) {
    if (eg && eg == fg) {
        return true;
    }

    return false;
}


bool SealGateOption::execute()
{
    ca.m_bSeal = true;
    //gGame->context().setGate(gGame->context().player()->getCharacter()->field()->gate());
    return GameOption::execute();
}

QString SealGateOption::sourceId() const
{
    if (gGame->context().gate())
        return gGame->context().gate()->id();
    return QString::null;
}

Cost SealGateOption::costs() const
{
    int amount = gGame->context().getGameProperty(PropertyValue::Game_SealClueCost).finalVal();
    CostList l;
    l << CostItem(CostItem::Pay_Clue, amount);
    Cost c;
    c.addAlternative(l);
    return c;
}

bool SealGateOption::isAvailable() const
{
    const Gate *eg = gGame->context().player()->getCharacter()->getExploredGate();
    const Gate *fg = gGame->context().gate();

    if (eg && eg == fg) {
        return true;
    }
    return false;
}
