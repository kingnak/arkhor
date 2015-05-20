#include "gamefield.h"
#include "../monster.h"
#include "../gate.h"
#include "../character.h"

GameField::GameField(AH::Common::FieldData::FieldID id, const QString &name, AH::Common::FieldData::FieldType type)
:   m_gate(NULL),
    m_whiteField(NULL),
    m_blackField(NULL)
{
    m_id = id;
    m_name = name;
    m_type = type;
}

void GameField::putClue(int amount)
{
    m_clues += amount;
}

int GameField::takeClues()
{
    int ret = m_clues;
    m_clues = 0;
    return ret;
}

void GameField::addBackGate(Gate *p)
{
    Q_ASSERT_X(m_type == AH::Common::FieldData::OtherWorld, "Add Back Gate", "Can only add Back Gates to Other Worlds");
    Q_ASSERT_X(!m_backGates.contains(p), "Add Back Gate", "Already have that gate");
    m_backGates.append(p);
}

void GameField::removeGate(Gate *p)
{
    if (m_gate == p) {
        m_gate = NULL;
    }

    m_backGates.removeAll(p);
}

void GameField::setGate(Gate *p)
{
    Q_ASSERT_X(m_gate == NULL || p == NULL, "Set Gate", "Already have gate");
    Q_ASSERT_X(m_type == AH::Common::FieldData::Location, "Set Gate", "Can onlx add gate to Interior");
    m_gate = p;
    if (p) p->setField(this);
}

bool GameField::isConnectedTo(GameField *other) const
{
    return m_neighbours.contains(other);
}

void GameField::placeCharacter(Character *c)
{
    if (c) {
        if (c->field()) {
            c->field()->removeCharacter(c);
        }

        m_characters.append(c);
        c->setField(this);
    }
}

void GameField::placeMonster(Monster *m)
{
    if (m) {
        if (m->field()) {
            m->field()->removeMonster(m);
        }

        m_monsters.append(m);
        m->setField(this);
    }
}

void GameField::removeCharacter(Character *c)
{
    if (c) {
        if (c->field() == this) {
            m_characters.removeAll(c);
            c->setField(NULL);
        }
    }
}

void GameField::removeMonster(Monster *m)
{
    if (m) {
        if (m->field() == this) {
            m_monsters.removeAll(m);
            m->setField(NULL);
        }
    }
}


// Overloads for GameFieldData:
QStringList GameField::characterIds() const
{
    QStringList ret;
    foreach (Character *c, m_characters) {
        ret << c->id();
    }
    return ret;
}

QList<int> GameField::neighbourIds() const
{
    QList<int> ret;
    foreach (GameField *f, m_neighbours) {
        ret << f->id();
    }
    return ret;
}
