#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "gamefield.h"
#include "movementpath.h"
#include <ahglobal.h>
#include <QMap>

class GameBoard
{
public:
    GameBoard();

    void init();

    bool validatePath(MovementPath p);

    void unlockFields(quint32 unlockFlags);

    GameField *field(AH::Common::FieldData::FieldID id) { return m_fields[id]; }

    QList<GameField *> fields(AH::Common::FieldData::FieldType type);
    QList<GameField *> allFields();

    void setDirty(bool dirty = true) { m_dirty = dirty; m_boardCacheDirty = true; }
    bool isDirty() const { return m_dirty; }

    AH::OtherWorldColors colorsForOtherWorld(AH::Common::FieldData::FieldID id) const;

    QList<Monster *> getBoardMonsters();
    QList<Gate *> getGates();

private:
    QMap<int, GameField *> m_fields;

    void connectfield(AH::Common::FieldData::FieldID src, AH::Common::FieldData::FieldID dst, bool w, bool b);
    void addField(GameField *f);

    void updateCaches();

private:
    bool m_dirty;
    // All "movable" monsters, so all except outskrits (and other world, but there won't be permanent monsters)
    QList<Monster *> m_boardMonsterCache;
    QList<Gate *> m_gateCache;
    bool m_boardCacheDirty;
};

#endif // GAMEBOARD_H
