#ifndef GATE_H
#define GATE_H

#include <ahglobal.h>
#include <gatedata.h>

class GameField;
class Character;

class Gate : public AH::Common::GateData
{
public:
    Gate(AH::Dimensions dim, int closeAdjustment, GameField *dest);

    //QString id() const { return "GA_Dummy"; }

    void setId(const QString &id) { m_id = id; }

    virtual AH::Common::FieldData::FieldID source() const;
    virtual AH::Common::FieldData::FieldID destination() const;

    GameField *sourceField() { return m_sourceField; }
    void setSourceField(GameField *field) { m_sourceField = field; }
    GameField *destinationField() { return m_destField; }

    void open();

    void enter(Character *c);
    void comeBack(Character *c);
    void close(Character *c);
    void seal(Character *c);

private:
    GameField *m_destField;
    GameField *m_sourceField;
};

#endif // GATE_H
