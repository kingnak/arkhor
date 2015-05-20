#include "otherworldencounteryaction.h"
#include "../game.h"
#include "character.h"
#include "../player.h"

OtherWorldEncounteryAction::OtherWorldEncounteryAction()
{
}

bool OtherWorldEncounteryAction::execute()
{
    // Assume succesful
    return true;
}


bool OtherWorldEncounteryOption::isAvailable()
{
    return gGame->context().player()->getCharacter()->field()->type() == AH::Common::FieldData::OtherWorld;
}
