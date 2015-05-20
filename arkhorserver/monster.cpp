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

void Monster::move(AH::MovementDirection dir)
{
    // Don't move if not on board
    if (!m_field) {
        return;
    }

    // Never move away from characters
    if (m_field->hasCharacters()) {
        return;
    }

    GameField *curField = m_field;
    switch (m_movement) {
    case Stationary:
        return;
    case Fast:
        // Move 2 fields
        if (dir == AH::MoveBlack) {
            curField = curField->blackField();
        } else {
            curField = curField->whiteField();
        }
        // Stop after 1 field if there is a character
        if (curField->hasCharacters()) {
            curField->placeMonster(this);
            break;
        }
        // Fallthrough to move to next field
    case Normal:
        if (dir == AH::MoveBlack) {
            curField = curField->blackField();
        } else {
            curField = curField->whiteField();
        }
        curField->placeMonster(this);
        break;

    case Flying:
        if (curField->type() == AH::Common::FieldData::Street) {
            // If on street, there will be no character (this is handled above), so goto sky
            curField = gGame->board()->field(AH::Common::FieldData::Sp_Sky);
        } else if (curField->type() == AH::Common::FieldData::Location) {
            // If on location, move to neighbouring street if there is a character
            // (this should be the white AND black direction). If not, goto sky
            Q_ASSERT_X(curField->blackField()->type() == AH::Common::FieldData::Street, "Monster::move Flying", "Black Neighbour of Location should be a street");
            curField = curField->blackField();
            if (!curField->hasCharacters()) {
                curField = gGame->board()->field(AH::Common::FieldData::Sp_Sky);
            } // Else stay

        } else if (curField->type() == AH::Common::FieldData::Sky) {
            // If in sky, find a street with character
            QList<GameField *> streets;
            foreach (GameField *s, gGame->board()->fields(AH::Common::FieldData::Street)) {
                if (s->hasCharacters()) {
                    // Candidate
                    streets << s;
                }
            }

            // TODO: Shuffle candidates, to make it less predictable if two characters
            // with lowest stamina are on steets


            // Find street with weakest character (lowest stamina)
            GameField *r = NULL;
            int weakest = 1000;
            foreach (GameField *s, streets) {
                foreach (Character *c, s->characters()) {
                    if (c->curStamina() < weakest) {
                        r = s;
                        weakest = c->curStamina();
                        break;
                    }
                }
            }

            if (r) {
                curField = r;
            }
        } else {
            Q_ASSERT_X(false, "Monster::move flying", qPrintable(QString("Monster was on unhandled field type %1").arg(m_field->type())));
        }

        if (curField != m_field) {
            curField->placeMonster(this);
        }
        break;

    case Special:
        Q_ASSERT_X(false, "Monster::move", "Cannot handle special movement here");
    }
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
        if (m_field) {
            m_field->removeMonster(this);
        }
        // Back to monster pool
        gGame->returnMonster(this);
    }
}
