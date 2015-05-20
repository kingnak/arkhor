#include "ancientone.h"

AncientOne::AncientOne()
    : m_doomValue(0), m_awake(false), m_dirty(false)
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
    m_awake = true;
}
