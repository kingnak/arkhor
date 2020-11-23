#ifndef ANCIENTONEATTACK_H
#define ANCIENTONEATTACK_H

#include "game/gamephase.h"

class AncientOneAttackPhase : public GamePhase
{
public:
    AncientOneAttackPhase(Game *g);

    void execute() override;

private:
    bool m_firstRound;

};



#endif // ANCIENTONEATTACK_H
