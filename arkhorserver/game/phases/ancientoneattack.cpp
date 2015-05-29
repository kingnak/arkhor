#include "ancientoneattack.h"
#include "ancientone.h"

AncientOneAttackPhase::AncientOneAttackPhase(Game *g)
    : GamePhase(g), m_firstRound(true)
{
}

void AncientOneAttackPhase::execute()
{
    gGame->ancientOne()->attack();
}
