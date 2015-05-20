#include "placeonfieldaction.h"
#include "game/game.h"
#include "game/player.h"
#include "character.h"

PlaceOnFieldAction::PlaceOnFieldAction()
    : m_field(NULL)
{
}

bool PlaceOnFieldAction::execute()
{
    if (!m_field) return false;
    m_field->placeCharacter(gGame->context().player()->getCharacter());
    return true;
}


bool PlaceOnFieldOption::isAvailable() const
{
    return pofa.m_field != NULL;
}
