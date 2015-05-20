#include "characterscript.h"

CharacterScript::CharacterScript(Investigator *i, QObject *parent) :
    QObject(parent), Character(i)
{
}

int CharacterScript::getSkillValue(int skill)
{
    AH::Skill sk = static_cast<AH::Skill> (skill);
    return gGame->context().getCharacterSkill(this, sk).finalVal();
}

