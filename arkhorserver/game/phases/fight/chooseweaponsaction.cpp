#include "chooseweaponsaction.h"
#include "game/gameobject.h"
#include "game/game.h"
#include "game/player.h"
#include "character.h"
#include "fightphase.h"

ChooseWeaponsAction::ChooseWeaponsAction(FightPhase *fight)
    : m_fight(fight)
{
}


bool ChooseWeaponsAction::execute()
{
    QList<GameObject *> lst = gGame->context().player()->getCharacter()->inventory();
    QList<GameObject *> fightObjects;
    foreach (GameObject *o, lst) {
        if (o->isFightObject()) {
            fightObjects << o;
        }
    }

    // TODO: Let user choose

    m_fight->updatePhaseByResult(FightPhase::WeaponsChosen);
    return true;
}
