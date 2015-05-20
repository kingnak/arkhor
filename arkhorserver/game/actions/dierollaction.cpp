#include "dierollaction.h"
#include "game/dietesthelper.h"
#include "game/game.h"
#include "die/dierollcountevaluator.h"

bool DieRollOption::isAvailable() const
{
    if (m_used)
        return false;
    if (!gGame->context().dieRoll())
        return false;
    AH::Common::PropertyValueData prop = gGame->context().dieRoll()->data.rollData().pool().property().property();
    if (prop.property() == AH::Common::PropertyValueData::NoProperty)
        return false;
    AH::Skill sk = PropertyValue::property2Skill(prop.property());
    if (!m_skills.testFlag(sk))
        return false;
    return true;
}

bool DieRollOption::execute()
{
    if (m_used) {
        return false;
    }
    m_used = true;

    DieTestHelper::DieTestSpec *spec = gGame->context().dieRoll();
    DieRollCountEvaluator *drce = dynamic_cast<DieRollCountEvaluator *> (spec->eval);
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
    }

    return true;
}

QString DieRollOption::name() const
{
    switch (m_type) {
    case ReRollAll: return "Reroll all";
    case ReRollAllFailed: return "Reroll all failed";
    case ReRollOneFailed: return "Reroll one die";
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
    }
    Q_ASSERT_X(false, "DieRollOption", "Invalid type");
    return "";
}