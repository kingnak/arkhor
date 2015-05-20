#ifndef MYTHOS_H
#define MYTHOS_H

#include "game/gamephase.h"
#include "game/game.h"
#include "game/gameaction.h"

class MythosAction : public GameAction
{
public:
    MythosAction() {
        m_phases = AH::Mythos;
    }
    ~MythosAction() {}

    bool execute();

    QString id() const { return "AC_MYTHOS"; }
};

class Mythos : public GamePhase
{
public:
    Mythos(Game *game);
    ~Mythos();

protected:
    QList<GameAction *> getPhaseActions();
    //void enterPhase();

private:
    MythosAction *m_mythos;
};

#endif // MYTHOS_H
