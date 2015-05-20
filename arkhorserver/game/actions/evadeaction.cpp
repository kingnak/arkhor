#include "evadeaction.h"
#include "../game.h"
#include "character.h"
#include "../player.h"

EvadeAction::EvadeAction()
    :  m_hasFailedAttempt(false)
{
}

bool EvadeAction::execute()
{
    m_hasFailedAttempt = true;
    return true;
}

bool EvadeOption::isAvailable()
{
    return gGame->context().player()->getCharacter()->field()->hasMonsters() && !sa.m_hasFailedAttempt;
}
