#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <gamefielddata.h>
#include <QString>
#include <QSet>
#include <QList>

class Character;
class Monster;
class Gate;
class GameOption;

class GameField : public AH::Common::GameFieldData
{
public:
    GameField(AH::Common::FieldData::FieldID id, const QString &name, AH::Common::FieldData::FieldType type);

    static const quint32 LOCK_PERMANENT = 0x1000;
    static const quint32 LOCK_TERROR = 0x0800;

    void putClue(int amount = 1);
    int takeClues();

    bool hasCharacters() const { return !m_characters.isEmpty(); }
    QList<Character *> characters() { return m_characters; }
    bool hasMonsters() const { return !m_monsters.isEmpty(); }
    QList<Monster *> monsters() { return m_monsters; }
    Gate *gate() { return m_gate; }

    QList<Gate *> backGates() { return m_backGates; }
    void addBackGate(Gate *p);
    void removeGate(Gate *p);
    void setGate(Gate *p);

    void setSealed(bool sealed);

    void lock(quint32 lockFlag);
    void unlock(quint32 lockFlag);
    bool isLocked() const;

    QSet<GameField *> neighbours() { return m_neighbours; }
    GameField *whiteField() { return m_whiteField; }
    GameField *blackField() { return m_blackField; }

    QList<GameOption *> getFieldOptions() { return m_options; }
    void addFieldOption(GameOption *o) { m_options.append(o); }
    void removeFieldOption(GameOption *o) { m_options.removeAll(o); }

    void setSpecialActionNr(int nr);
    void unsetSpecialActionNr();

    bool isConnectedTo(GameField *other) const;
    bool isConnectedTo(int other) const;

    void placeCharacter(Character *c);
    void placeMonster(Monster *m);

    void removeCharacter(Character *c);
    void removeMonster(Monster *m);


    virtual QStringList characterIds() const;
    virtual QStringList monsterIds() const;
    virtual QString gateId() const;
    virtual QList<int> neighbourIds() const;

private:
    QSet<GameField *> m_neighbours;
    QList<Character *> m_characters;
    QList<Monster *> m_monsters;
    QList<GameOption *> m_options;
    Gate *m_gate;
    QList<Gate *> m_backGates;
    GameField *m_whiteField;
    GameField *m_blackField;

    quint32 m_lockFlags;

    friend class GameBoard;
};

#endif // GAMEFIELD_H
