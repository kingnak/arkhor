#include "dierollaction.h"
#include "game/dietesthelper.h"
#include "game/game.h"
#include "die/dierollcountevaluator.h"
#include "game/player.h"
#include "character.h"

quint32 DieRollOption::s_nextId = 0;

bool DieRollOption::isAvailable() const
{
    if (m_used)
        return false;
    if (m_source && m_source->isExhausted())
        return false;
    if (!gGame->context().dieRoll())
        return false;
    AH::Skill skill = gGame->context().dieRoll()->data.rollData().skill();
    if (skill == AH::NoSkill)
        return false;
    if (!m_skills.contains(skill))
        return false;
    return true;
}

bool DieRollOption::execute()
{
    if (!costs().getAlternatives().empty()) {
        if (!gGame->context().player()->getCharacter()->pay(costs(), m_source?m_source->id():"", description())) {
            return false;
        }
        // Inform about payment
        gGame->commitUpdates();
    }

    if (m_used) {
        return false;
    }
    m_used = true;

    DieTestHelper::DieTestSpec *spec = gGame->context().dieRoll();
    DieRollCountEvaluator *drce = dynamic_cast<DieRollCountEvaluator *> (spec->eval);
    DieRollBoolEvaluator *drbe = dynamic_cast<DieRollBoolEvaluator *> (spec->eval);
    switch (type()) {
    case DieRollOption::ReRollAll:
        spec->eval->rerollAll();
        break;
    case DieRollOption::ReRollAllFailed:
        if (drce) {
            drce->rerollFailed();
        }
        break;
    case DieRollOption::ReRollOneFailed:
        if (drce) {
            drce->rerollNumFailed(1);
        }
        break;
    case DieRollOption::AutoSucceed:
        if (drbe) {
            drbe->succeed();
        }
        break;
    }

    if (m_source) {
        m_source->exhaust();
        if (m_discardAfterUse) {
            m_source->returnToDeck();
        }
    }

    return true;
}

QString DieRollOption::name() const
{
    switch (m_type) {
    case ReRollAll: return "Reroll all";
    case ReRollAllFailed: return "Reroll all failed";
    case ReRollOneFailed: return "Reroll one die";
    case AutoSucceed: return "Succeed";
    }
    Q_ASSERT_X(false, "DieRollOption", "Invalid type");
    return "";
}

QString DieRollOption::description() const
{
    switch (m_type) {
    case ReRollAll: return "Reroll all dice";
    case ReRollAllFailed: return "Reroll all failed dice";
    case ReRollOneFailed: return "Reroll one die";
    case AutoSucceed: return "Automatically succeed";
    }
    Q_ASSERT_X(false, "DieRollOption", "Invalid type");
    return "";
}

QString DieRollOption::sourceId() const
{
    if (m_source) {
        return m_source->id();
    }
    return GameOption::sourceId();
}
