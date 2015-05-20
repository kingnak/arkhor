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
    Gate *g = c->field()->gate();
    return c->getExploredGate() == NULL && g != NULL;
}
