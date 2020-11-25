#include "player.h"
#include "gameobject.h"
#include "gameaction.h"
#include "gameoption.h"
#include "game.h"

Player::Player()
    : m_active(true)
    , m_character(nullptr)
{
}

Character *Player::getCharacter()
{
    return m_character;
}
