#include "movementpath.h"

GameField *MovementPath::startField()
{
    return first();
}

GameField *MovementPath::endField()
{
    return last();
}

int MovementPath::length()
{
    if (empty()) return 0;
    return size() - 1;
}
