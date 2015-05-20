#include "monster.h"
#include "game/gamefield.h"
#include "character.h"

AH::Common::MonsterData *Monster::data()
{
    // Synchronize data
    m_fieldId = (m_field) ? m_field->id() : AH::Common::FieldData::NO_NO_FIELD;
    m_attributes = attributes();
    return AH::Common::MonsterData::data();
}

void Monster::move()
{

}

void Monster::defeat(Character *byCharacter)
{
    if (m_field) {
        m_field->removeMonster(this);
    }

    if (attributes().testFlag(Endless)) {
        // Back to monter pool
        gGame->returnMonster(this);
    } else {
        byCharacter->addMonsterMarker(this);
    }
}

void Monster::endCombat()
{
    if (isSpontaneous()) {
        // Back to monster pool
        gGame->returnMonster(this);
    }
}
