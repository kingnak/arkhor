#include "characterscript.h"

CharacterScript::CharacterScript(Investigator *i, QObject *parent) :
    QObject(parent), Character(i)
{
    m_fieldBridge = new GameFieldScript(this);
}

GameFieldScript *CharacterScript::fieldScript()
{
    m_fieldBridge->setField(m_field);
    return m_fieldBridge;
}

int CharacterScript::getSkillValue(int skill)
{
    AH::Skill sk = static_cast<AH::Skill> (skill);
    return gGame->context().getCharacterSkill(this, sk).finalVal();
}

bool CharacterScript::placeOnField(int fld)
{
    AH::Common::FieldData::FieldID fid = static_cast<AH::Common::FieldData::FieldID>(fld);
    GameField *field = gGame->board()->field(fid);
    if (field) {
        field->placeCharacter(this);
        return true;
    }
    return false;
}

bool CharacterScript::canCloseGate()
{
    if (m_explorededGate && m_explorededGate == gGame->context().gate()) {
        return true;
    }
    return false;
}

