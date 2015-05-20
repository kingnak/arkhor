#include "fightaction.h"
#include "game/game.h"
#include "character.h"
#include "game/player.h"
#include "fightphase.h"
#include "monster.h"

FightAction::FightAction(FightPhase *fight)
    : m_fight(fight)
{
}

bool FightAction::execute()
{
    /*
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
    */

    // Just a marker...
    m_fight->updatePhaseByResult(FightPhase::FightChosen);
    return true;
}


QString FightOption::sourceId() const
{
    return gGame->context().monster()->id();
}

bool FightOption::isAvailable() const
{
    //return gGame->context().player()->getCharacter()->field()->hasMonsters();
    return gGame->context().monster() != NULL;
}
