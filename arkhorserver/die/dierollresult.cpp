#include "die/dierollresult.h"

DieRollResult::DieRollResult()
{
}

void DieRollResult::add(DiePool::DiePoolIndex idx, quint32 value)
{
    this->append(DieRollResultItem(idx, value));
}
