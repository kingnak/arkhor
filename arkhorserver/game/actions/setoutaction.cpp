#include "setoutaction.h"
#include "game/game.h"
#include "game/player.h"
#include "character.h"

SetOutAction::SetOutAction()
{
}

bool SetOutAction::execute()
{
    if (gGame->context().player()->getCharacter()->isSetOut()) {
        gGame->context().player()->getCharacter()->setSetout(false);
    }
    return true;
}
