#ifndef MONSTER_H
#define MONSTER_H

#include <QString>

class GameField;

class Monster
{
public:
    Monster() : m_field(NULL) {}
    virtual ~Monster() {}

    QString name() const { return "Monster"; }

    GameField *field() { return m_field; }
    void setField(GameField *f) { m_field = f; }

protected:
    GameField *m_field;
};

#endif // MONSTER_H
