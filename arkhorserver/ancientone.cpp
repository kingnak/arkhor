#include "ancientone.h"
#include "game/game.h"

AncientOne::AncientOne()
    : m_awake(false), m_dirty(false), m_damageThreshold(1), m_damage(0)
{
}

void AncientOne::increaseDoomTrack(int amount)
{
    m_doomValue += amount;
    setDirty();
}

void AncientOne::awake()
{
    m_doomValue = m_doomTrack;
    m_damageThreshold = gGame->getPlayers().count();
    m_damage = 0;
    m_awake = true;
}

void AncientOne::newAttackRound()
{
    decreaseAttackAdjustment(1);
}

void AncientOne::damage(int amount)
{
    m_damage += amount;
    int dec = m_damage / m_damageThreshold;
    m_damage %= m_damageThreshold;
    increaseDoomTrack(-dec);
}
