#ifndef GAMEOPTION_H
#define GAMEOPTION_H

#include "gameaction.h"
#include <cost.h>
#include <gameoptiondata.h>

class Game;

class GameOption : public AH::Common::GameOptionData
{
public:
    GameOption(GameAction *action = NULL, AH::ContinueType cont = AH::CanContinue, AH::ChooseType choose = AH::ChooseOptional, AH::GamePhases phases = AH::NoGamePhase)
        : m_action(action)
    {
        m_chooseType = choose;
        m_continueType = cont;
        m_phases = phases;
        if (m_action && m_phases == AH::NoGamePhase) {
            m_phases = m_action->phases();
        }
    }
    virtual ~GameOption() {}

    virtual AH::GamePhases phases() const;

    virtual bool isAvailable() = 0;

    virtual bool execute();

    QString name() const;
    QString description() const;

    virtual bool resolveDependencies(const Game *game);

//    virtual bool disposeAfterExecute() { return false; }

protected:
    GameAction *m_action;
};

#endif // GAMEOPTION_H
