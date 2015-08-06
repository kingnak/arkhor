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
        if (p->isActive()) {
            if (sel) {
                // Remove from game's pool
                gGame->drawSpecificInvestigator(sel->id());
            } else {
                // None chosen, use random
                sel = gGame->drawInvestigator();
            }
            Character *c = sel->instantiate();
            p->setCharacter(c);
            gGame->registerCharacter(c);
            inv.removeAll(sel);
            p->playerCharacterInstantiated(p);
        }
    }
}
