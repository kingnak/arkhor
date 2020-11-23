#include "chooseskillhelper.h"
#include "character.h"
#include "game/game.h"
#include "game/player.h"


AH::Skill ChooseSkillHelper::chooseSkill(Player *p, const QList<AH::Skill> &options)
{
    QList<AH::Common::ModifiedPropertyValueData> opts;

    for (auto s : options) {
        ModifiedPropertyValue v = gGame->context().getCharacterSkill(p->getCharacter(), s);
        opts.append(v.toModifiedPropertyValueData());
    }

    AH::Common::PropertyValueData::Property ret = p->chooseSkill(opts);
    AH::Skill s = PropertyValue::property2Skill(ret);
    return s;
}

AH::Skill ChooseSkillHelper::chooseSkillDieRoll(Player *p, const QList<AH::Skill> &options)
{
    QList<AH::Common::ModifiedPropertyValueData> opts;

    for(AH::Skill s : options) {
        ModifiedPropertyValue v = gGame->context().getCharacterDieRoll(p->getCharacter(), PropertyValue::skill2DieRoll(s));
        opts.append(v.toModifiedPropertyValueData());
    }

    AH::Common::PropertyValueData::Property ret = p->chooseSkill(opts);
    AH::Skill s = PropertyValue::property2Skill(ret);
    return s;
}
