#ifndef GAMEACTION_H
#define GAMEACTION_H

#include <ahglobal.h>
#include <QString>
#include <gameactiondata.h>

class GameAction : public AH::Common::GameActionData
{
public:
    GameAction() {}
    virtual ~GameAction() {}

    virtual bool execute() = 0;

    void setId(QString id) { m_id = id; }
};

#endif // GAMEACTION_H
