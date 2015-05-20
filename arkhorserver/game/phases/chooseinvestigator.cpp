#include "chooseinvestigator.h"
#include "game/investigator.h"
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
                Character *c = sel->instantiate();
                p->setCharacter(c);
                inv.removeAll(sel);
            }
        }
    }
}
