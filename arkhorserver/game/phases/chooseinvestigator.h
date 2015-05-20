#ifndef CHOOSEINVESTIGATOR_H
#define CHOOSEINVESTIGATOR_H

#include "game/gamephase.h"

class ChooseInvestigator : public GamePhase
{
public:
    ChooseInvestigator(Game *game);

    void execute();

private:
    Character *instantiateInvestigator(Investigator *i);
};

#endif // CHOOSEINVESTIGATOR_H
