#ifndef MONSTER_H
#define MONSTER_H

#include <QString>
#include <monsterdata.h>

class Character;
class GameField;

class Monster : public AH::Common::MonsterData
{
public:
    Monster() : m_field(NULL), m_isSpontaneous(false) {}
    virtual ~Monster() {}

    virtual Monster *clone() = 0;

    virtual void setId(QString id) { m_id = id; }

    virtual MonsterData *data();

    GameField *field() { return m_field; }
    void setField(GameField *f) { m_field = f; }

    bool isSpontaneous() const { return m_isSpontaneous; }
    void setSpontaneous(bool spontaneous = true) { m_isSpontaneous = spontaneous; }

    virtual void move() = 0;
    virtual void defeat(Character *byCharacter);
    virtual void endCombat();

protected:
    GameField *m_field;
    bool m_isSpontaneous;
};

#endif // MONSTER_H
