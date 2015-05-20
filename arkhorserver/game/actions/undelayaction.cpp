#include "undelayaction.h"
#include "game/game.h"
#include "game/player.h"
#include "character.h"

UndelayAction::UndelayAction()
{
}

bool UndelayAction::execute()
{
    if (gGame->context().player()->getCharacter()->isDelayed()) {
        gGame->context().player()->getCharacter()->setDelayed(false);
    }
    return true;
}


bool UndelayOption::isAvailable() const
{
    return gGame->context().player()->getCharacter()->isDelayed();
}
