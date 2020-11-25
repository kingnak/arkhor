#include "die/dierollresult.h"

DieRollResult::DieRollResult() = default;

void DieRollResult::add(DiePool::DiePoolIndex idx, quint32 value)
{
    this->append(DieRollResultItem(idx, value));
}
