#include "evadeaction.h"
#include "game/game.h"
#include "character.h"
#include "game/player.h"
#include "game/dietesthelper.h"
#include "monster.h"
#include "fightphase.h"

EvadeAction::EvadeAction(FightPhase *fight)
    :  m_hasFailedAttempt(false), m_fight(fight)
{
    m_name = "Evade";
}

bool EvadeAction::execute()
{
    Monster *m = gGame->context().monster();
    Player *p = gGame->context().player();

    gGame->notifier()->actionStart(this);
    DieTestHelper::DieTestSpec test = DieTestHelper::createSkillTest(name() + " from monster", m->id(), p->getCharacter(), AH::Skill_Evade, m->awareness());
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(p, test);
    if (res.boolResult) {
        gGame->notifier()->actionUpdate(this, "succeded");
        m_fight->updatePhaseByResult(FightPhase::CharacterFlown);
    } else {
        gGame->notifier()->actionUpdate(this, "failed");
        m_fight->updatePhaseByResult(FightPhase::EvadeFailed);
    }
    gGame->notifier()->actionFinish(this);
    return true;
}

QString EvadeAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(desc);
    switch (part) {
    case Start: return "{C} tries to evade {M}";
    case Finish: return "Evade {B}";
    default:
        return QString::null;
    }
}

bool EvadeOption::isAvailable() const
{
    Monster *m = gGame->context().monster();
    if (m->attributes().testFlag(AH::Common::MonsterData::Ambush)) {
        // Cannot evade
        return false;
    }
    return true;
}

QString EvadeOption::sourceId() const
{
    return gGame->context().monster()->id();
}

AH::Common::ModifiedPropertyValueData EvadeOption::baseProperty() const
{
    return gGame->context().getCurCharacterDieRoll(PropertyValue::Property::DieRoll_Evade).toModifiedPropertyValueData();
}
