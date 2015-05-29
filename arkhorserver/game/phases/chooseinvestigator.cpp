#include "chooseinvestigator.h"
#include "game/investigator.h"
#include "character.h"
#include "../player.h"

ChooseInvestigator::ChooseInvestigator(Game *game)
    : GamePhase(game)
{
}

void ChooseInvestigator::execute()
{
    QList<Investigator *> inv = gGame->allInvestigators();
    foreach (Player *p, gGame->getPlayers()) {
        if (!p->isActive()) continue;
        Investigator *sel = p->chooseInvestigator(inv);
        if (sel) {
            if (p->isActive()) {
                // Remove from game's pool
                gGame->drawSpecificInvestigator(sel->id());

                Character *c = sel->instantiate();
                p->setCharacter(c);
                gGame->registerCharacter(c);
                inv.removeAll(sel);
                p->playerCharacterInstantiated(p);
            }
        } else {
            // TODO: None chosen, use random
        }
    }
}
