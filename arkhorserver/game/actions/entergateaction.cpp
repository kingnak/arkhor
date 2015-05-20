#include "entergateaction.h"
#include "../game.h"
#include "../../gate.h"
#include "character.h"
#include "../player.h"

EnterGateAction::EnterGateAction()
{
}

bool EnterGateAction::execute()
{
    gGame->context().player()->getCharacter()->field()->gate()->enter(gGame->context().player()->getCharacter());
    return true;
}


bool EnterGateOption::isAvailable()
{
    Character *c = gGame->context().player()->getCharacter();
    return c->getExploredGate() == NULL && c->field()->gate() != NULL;
}
