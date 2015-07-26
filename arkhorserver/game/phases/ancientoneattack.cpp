#include "ancientoneattack.h"
#include "ancientone.h"
#include "game/player.h"
#include "character.h"

AncientOneAttackPhase::AncientOneAttackPhase(Game *g)
    : GamePhase(g), m_firstRound(true)
{
}

void AncientOneAttackPhase::execute()
{
    gGame->ancientOne()->attack();
    gGame->context().player()->getCharacter()->commitDamage();
    gGame->commitUpdates();
}
