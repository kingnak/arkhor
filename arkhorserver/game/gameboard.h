#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "gamefield.h"
#include "movementpath.h"
#include <QMap>

class GameBoard
{
public:
    GameBoard();

    void init();

    bool validatePath(MovementPath p);


    GameField *field(AH::Common::FieldData::FieldID id) { return m_fields[id]; }

    QList<GameField *> fields(AH::Common::FieldData::FieldType type);
    QList<GameField *> allFields();

    void setDirty(bool dirty = true) { m_dirty = dirty; }
    bool isDirty() const { return m_dirty; }

private:
    QMap<int, GameField *> m_fields;

    void connectfield(AH::Common::FieldData::FieldID src, AH::Common::FieldData::FieldID dst, bool w, bool b);
    void addField(GameField *f);

    bool m_dirty;
};

#endif // GAMEBOARD_H
