#ifndef MOVEMENTPATH_H
#define MOVEMENTPATH_H

#include "gamefield.h"
#include <QList>

class MovementPath : public QList<GameField *>
{
public:
    GameField *startField();
    GameField *endField();
    int length();
};

#endif // MOVEMENTPATH_H
