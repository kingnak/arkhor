#include "horroraction.h"
#include "game/game.h"
#include "character.h"
#include "game/player.h"
#include "game/dietesthelper.h"
#include "monster.h"
#include "fightphase.h"

HorrorAction::HorrorAction(FightPhase *fight)
    : m_fight(fight)
{
}


bool HorrorAction::execute()
{
    Monster *m = gGame->context().monster();
    Player *p = gGame->context().player();

    if (m->horrorDamage() > 0) {
        gGame->notifier()->actionStart(this);
        DieTestHelper::DieTestSpec test = DieTestHelper::createSkillTest("Horror Check", m->id(), p->getCharacter(), AH::Skill_Horror, m->horrorAdjustment());
        DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(p, test);
        if (res.boolResult) {
            gGame->notifier()->actionUpdate(this, "succeeded");
            m_fight->updatePhaseByResult(FightPhase::HorrorSuccess);
        } else {
            gGame->notifier()->actionUpdate(this, "failed");
            m_fight->updatePhaseByResult(FightPhase::HorrorFailed);
        }
        gGame->notifier()->actionFinish(this);
    } else {
        m_fight->updatePhaseByResult(FightPhase::HorrorSuccess);
    }
    return true;
}

QString HorrorAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(desc);
    switch (part) {
    case Start: return "{C} makes a horror test for {M}";
    case Finish: return "{C} {B} horror test";
    default: return QString::null;
    }
}


QString HorrorOption::sourceId() const
{
    return gGame->context().monster()->id();
}

bool HorrorOption::isAvailable() const
{
    Monster *m = gGame->context().monster();
    if (!m ||  m->horrorDamage() <= 0) {
        return false;
    }
    return true;
}
