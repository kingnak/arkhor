#include "fightaction.h"
#include "../game.h"
#include "character.h"
#include "../player.h"

FightAction::FightAction()
{
}

bool FightAction::execute()
{
    if (!gGame->context().monster())
    {
        // Cannot move further after a fight
        gGame->context().player()->getCharacter()->setMovementAmount(0);

        Monster *m = gGame->context().player()->getCharacter()->field()->monsters().at(0);
        if (m) {
            gGame->context().setMonster(m);
        }

        // Just kill it...
        gGame->context().setMonster(NULL);
        gGame->context().player()->getCharacter()->field()->removeMonster(m);
    }

    return true;
}


bool FightOption::isAvailable()
{
    return gGame->context().player()->getCharacter()->field()->hasMonsters();
}
