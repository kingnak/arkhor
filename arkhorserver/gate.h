#ifndef GATE_H
#define GATE_H

#include <ahglobal.h>

class GameField;
class Character;

class Gate
{
public:
    Gate(AH::Dimension dim, int closeAmount, GameField *dest);

    AH::Dimension dimension() const { return m_dim; }
    int closeAmount() const { return m_closeAmount; }

    GameField *field() { return m_field; }
    void setField(GameField *field) { m_field = field; }
    GameField *destination() { return m_dest; }

    void open();

    void enter(Character *c);
    void comeBack(Character *c);
    void close(Character *c);
    void seal(Character *c);

private:
    bool m_open;
    AH::Dimension m_dim;
    int m_closeAmount;
    GameField *m_dest;
    GameField *m_field;
};

#endif // GATE_H
